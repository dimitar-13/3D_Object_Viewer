#include "UI/UILayer.h"
#include "Camera.h"


OBJ_Viewer::Camera::Camera(float CameraZoom, int width, int height, Application& app):
	app(app)
{
	this->m_zoom = CameraZoom;
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)width / (float)height, 0.1f, 100.0f);
	//CameraZoom units from the origin or vec3(0,0,1) * CameraZoom;
	this->m_EulerAngles.m_pitchAngle =0.0f;
	this->m_EulerAngles.m_yawAngle = 90.0f;
	//Use to set the x_previous and y_previous of the 'm_EulerAngleHelper' so we can avoid conditions;
	m_EulerAngleHelper.calculateEulerAngles(this->m_EulerAngles.m_yawAngle,this->m_EulerAngles.m_pitchAngle);
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
	RecalculateViewMatrix();
}

void OBJ_Viewer::Camera::onMousePositionChanged(MousePositionEvent& e)
{
	auto& mousePos = e.GetMousePos();

	if (app.GetGlobalInputHandler().isMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		//We update the previous x and y position.
		m_EulerAngleHelper.calculateEulerAngles(mousePos.x, mousePos.y);
	}
	else if (app.GetGlobalInputHandler().isMouseButtonHeld(GLFW_MOUSE_BUTTON_1) && app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		m_EulerAngles += m_EulerAngleHelper.calculateEulerAngles(mousePos.x, mousePos.y);
		m_EulerAngleHelper.ConstrainAngles(m_EulerAngles);
		CalculatePositionVector();
	}

	if (app.GetGlobalInputHandler().isMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		//We update the previous x and y position.
		m_lastMousePos = glm::vec2(mousePos.x, mousePos.y);
	}
	else if (app.GetGlobalInputHandler().isMouseButtonHeld(GLFW_MOUSE_BUTTON_2) && app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
	{
		constexpr float movementSpeed = .05;
		glm::mat3 viewSpaceOrthogonalVectors = glm::mat3(m_viewMatrix);
		glm::vec3 viewDirectionVector(0);
		//We calculate the offset in camera/view space first.
		glm::vec2 delta = glm::vec2(mousePos.x, mousePos.y) - m_lastMousePos;
		viewDirectionVector.x = delta.x*movementSpeed;
		viewDirectionVector.y = delta.y*movementSpeed;
		//TL;DR: We use transpose as a cheap inverse since the "viewSpaceOrthogonalVectors" is a 3 orthogonal vector matrix
		//and then we bring our viewSpace delta into word space.
		/*Why we do it like this ? 
		* To pan/shift a camera we wish to move the screen up or down problem is that up/down and right/left is different for the camera
		* since the camera Z axis is always the cameras front in view space we know that there is a plane perpendicular on the camera Z
		* axis that we can move. To find where we camera up/down and right/left in world space we have to find this plane. The best approach is to 
		* calculate the movement in view space and then inverse the view matrix to get what the vector will be in world space.
		*/
		viewDirectionVector = glm::transpose(viewSpaceOrthogonalVectors) * viewDirectionVector;
		m_lastMousePos = glm::vec2(mousePos.x, mousePos.y);
		m_cameraCenter += viewDirectionVector;
		RecalculateViewMatrix();
	}

}

void OBJ_Viewer::Camera::onWinSizeChanged(WindowResizeEvent& e)
{
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)e.GetWindowWidth()/ (float)e.GetWindowHeight(), 0.1f, 100.0f);
}

void OBJ_Viewer::Camera::onKeyPressedEvent(KeyboardKeyEvent& e)
{
	if (e.GetKeyCode() == GLFW_KEY_Q)
	{
		m_cameraCenter = glm::vec3(0);
		RecalculateViewMatrix();
	}
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
	case EVENT_WINDOW_SIZE_CHANGED:
		this->onWinSizeChanged(dynamic_cast<WindowResizeEvent&>(e));
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
	default:
		break;
	}
}

glm::vec3 OBJ_Viewer::Camera::GetScreenToWorldSpacePos(glm::vec3 inPoint)
{
	auto sceneViewportSize = app.GetSceneViewport();
	auto winViewportSize = app.GetGlobalAppWindow().GetWindowSize();
		
	glm::mat4 result(1);
	result[0][0] = (float)sceneViewportSize.width /2;
	result[1][1] = (float)sceneViewportSize.height /2;

	result[2][0] = sceneViewportSize.x + result[0][0];
	result[2][1] = sceneViewportSize.y + result[1][1];



	glm::mat4 invViewport = glm::inverse(result);
	glm::mat4 invViewProjection = glm::inverse(this->GetViewProjMatrix());
	glm::vec4 point = glm::vec4(inPoint.x, inPoint.y,0.0, 1.0);
	point =  invViewProjection * invViewport * point;
	point /= point.w;

	return glm::vec3(point);
}


OBJ_Viewer::EulerAngles OBJ_Viewer::EulerAngleHelper::calculateEulerAngles(double xpos, double ypos)
{
	//Check if mouse button is held and then move;
	EulerAngles result = {static_cast<float>(xpos - m_previousXPos), static_cast<float>(m_previousYPos - ypos)};
	m_previousXPos = xpos;
	m_previousYPos = ypos;
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
