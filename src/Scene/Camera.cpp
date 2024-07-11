#include "UI/UILayer.h"
#include "Camera.h"

OBJ_Viewer::Camera::Camera(float CameraZoom, Size2D screenSize, Application& app):
	m_app(app)
{
	this->m_zoom = CameraZoom;
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)screenSize.width / (float)screenSize.height, 0.1f, 100.0f);
	//CameraZoom units from the origin or vec3(0,0,1) * CameraZoom;
	this->m_EulerAngles.m_pitchAngle =0.0f;
	this->m_EulerAngles.m_yawAngle = 90.0f;
	//Use to set the x_previous and y_previous of the 'm_EulerAngleHelper' so we can avoid conditions;
	m_EulerAngleHelper.calculateEulerAngles(Position2D{ this->m_EulerAngles.m_yawAngle,this->m_EulerAngles.m_pitchAngle });
	CalculatePositionVector();

}

void OBJ_Viewer::Camera::CalculatePositionVector()
{
	m_position.y = std::sin(glm::radians(this->m_EulerAngles.m_pitchAngle));
	m_position.z = std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) * std::sin(glm::radians(-this->m_EulerAngles.m_yawAngle));
	m_position.x = std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) * std::cos(glm::radians(-this->m_EulerAngles.m_yawAngle));
	m_position = glm::normalize(m_position);
	RecalculateViewMatrix();
}

void OBJ_Viewer::Camera::onScrollChanged(ScrollPositionChanged& e)
{
	constexpr float sensitivity = 0.01f;
	constexpr float maxZoom = 0.002;
	this->m_zoom += this->m_zoom - e.GetScrollPosition().y <= maxZoom ? 0 : -e.GetScrollPosition().y;

	if (!m_isProjectionPerspective)
	{
		Size2D windowSize = m_app.GetSceneFrameBuffer().GetFramebufferSize();
		CalculateOthoProjection(windowSize);
	}

	RecalculateViewMatrix();
}

void OBJ_Viewer::Camera::onMousePositionChanged(MousePositionEvent& e)
{
	auto& mousePos = e.GetMousePos();

	if (m_app.GetGlobalInputHandler().isMouseButtonPressed(MOUSE_KEY_1) && m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		//We update the previous x and y position.
		m_EulerAngleHelper.calculateEulerAngles(mousePos);
	}
	else if (m_app.GetGlobalInputHandler().isMouseButtonHeld(MOUSE_KEY_1) && m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		m_EulerAngles += m_EulerAngleHelper.calculateEulerAngles(mousePos);
		m_EulerAngleHelper.ConstrainAngles(m_EulerAngles);
		CalculatePositionVector();
	}

	if (m_app.GetGlobalInputHandler().isMouseButtonPressed(MOUSE_KEY_2) && m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		//We update the previous x and y position.
		m_lastMousePos = mousePos;
	}
	else if (m_app.GetGlobalInputHandler().isMouseButtonHeld(MOUSE_KEY_2) && m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		constexpr float movementSpeed = .005;
		glm::mat3 viewSpaceOrthogonalVectors = glm::mat3(m_viewMatrix);
		glm::vec3 viewDirectionVector(0);
		//We calculate the offset in camera/view space first.
		Position2D delta = Position2D{ mousePos.x - m_lastMousePos.x,mousePos.y - m_lastMousePos.y};
		viewDirectionVector.x = delta.x*movementSpeed;
		//We flip the y since opengl flips its y component 
		viewDirectionVector.y = -delta.y*movementSpeed;
		//TL;DR: We use transpose as a cheap inverse since the "viewSpaceOrthogonalVectors" is a 3 orthogonal vector matrix
		//and then we bring our viewSpace delta into word space.
		/*Why we do it like this ? 
		* To pan/shift a camera we wish to move the screen up or down problem is that up/down and right/left is different for the camera
		* since the camera Z axis is always the cameras front in view space we know that there is a plane perpendicular on the camera Z
		* axis that we can move. To find where we camera up/down and right/left in world space we have to find this plane. The best approach is to 
		* calculate the movement in view space and then inverse the view matrix to get what the vector will be in world space.
		*/
		viewDirectionVector = glm::transpose(viewSpaceOrthogonalVectors) * viewDirectionVector;
		m_lastMousePos = mousePos;
		m_cameraCenter += viewDirectionVector;
		RecalculateViewMatrix();
	}

}

void OBJ_Viewer::Camera::onWinSizeChanged(FramebufferResizeEvent& e)
{
	const Size2D winSize = e.GetNewFramebufferSize();
	RecalculateProjection(winSize);
}

void OBJ_Viewer::Camera::onKeyPressedEvent(KeyboardKeyEvent& e)
{
	if (e.GetKeyCode() == GLFW_KEY_Q)
	{
		m_cameraCenter = glm::vec3(0);
		RecalculateViewMatrix();
	}
}

void OBJ_Viewer::Camera::RecalculateProjection(Size2D windowSize)
{
	windowSize = (windowSize.width == 0 || windowSize.height == 0) ? m_app.GetSceneFrameBuffer().GetFramebufferSize() : windowSize;

	if (m_isProjectionPerspective)
		m_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)windowSize.width / (float)windowSize.height, 0.1f, 100.0f);
	else
	{
		CalculateOthoProjection(windowSize);
	}
}

void OBJ_Viewer::Camera::OnProjectionModeChanged(EventCameraProjectionChanged& e)
{
	m_isProjectionPerspective = e.isCameraProjectionPerspective();
	RecalculateProjection();
}

void OBJ_Viewer::Camera::CalculateOthoProjection(Size2D windowSize)
{
	float orthoScale = this->m_zoom / 500;
	/*Orthographic projection doesn't really have a zoom since all you see must stay true to its size so instead we shrink the
	* box defined by the left,right,bottom,top planes so that smaller values will get bigger meaning that since ortho matrix
	* is just a scale and translate we make the scale bigger by multiplying left,right,bottom,top with the 'orthoScale'.
	*/
	m_projectionMatrix = glm::ortho(-((float)windowSize.width / 2) * orthoScale, ((float)windowSize.width / 2) * orthoScale,
		-((float)windowSize.height / 2) * orthoScale, ((float)windowSize.height / 2) * orthoScale, -1.f/ orthoScale, 100.f);
}



void OBJ_Viewer::Camera::RecalculateViewMatrix()
{
	glm::vec3 upVec(0, 1, 0);
	upVec.y *= std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) < 0 ? -1 : 1;
	m_viewMatrix = glm::lookAt(m_position * m_zoom + m_cameraCenter, m_cameraCenter, upVec);
}

void OBJ_Viewer::Camera::OnEvent(Event& e)
{
	switch (e.GetEventType())
	{
	case EVENT_FRAMEBUFFER_SIZE_CHANGED:
		this->onWinSizeChanged(dynamic_cast<FramebufferResizeEvent&>(e));
		break;
	case EVENT_MOUSE_POSITION_CHANGED:
		this->onMousePositionChanged(dynamic_cast<MousePositionEvent&>(e));
		break;
	case EVENT_MOUSE_SCROLL_CHANGED:
		this->onScrollChanged(dynamic_cast<ScrollPositionChanged&>(e));
		break;
	case EVENT_KEY_PRESSES:
		this->onKeyPressedEvent(dynamic_cast<KeyboardKeyEvent&>(e));
		break;
	case EVENT_CAMERA_PROJECTION_TYPE_CHANGED:
		this->OnProjectionModeChanged(dynamic_cast<EventCameraProjectionChanged&>(e));
		break;
	default:
		break;
	}
}


OBJ_Viewer::EulerAngles OBJ_Viewer::EulerAngleHelper::calculateEulerAngles(Position2D newMousePosition)
{
	//Check if mouse button is held and then move;
	EulerAngles result = {static_cast<float>(newMousePosition.x - m_previousMousePosition.x),
		static_cast<float>(newMousePosition.y - m_previousMousePosition.y)};
	m_previousMousePosition = newMousePosition;
	ConstrainAngles(result);
	return result;
}

void OBJ_Viewer::EulerAngleHelper::ConstrainAngles(EulerAngles& angle)
{
	char sing = angle.m_pitchAngle > 0 ? -1 : 1;
	angle.m_pitchAngle += !(angle.m_pitchAngle >= -360.0f && angle.m_pitchAngle <= 360.0f) ? sing * 360.0f : 0;
	sing = angle.m_yawAngle > 0 ? -1 : 1;
	angle.m_yawAngle += !(angle.m_yawAngle >= -360.0f && angle.m_yawAngle <= 360.0f) ? sing * 360.0f : 0;
}
