#include "pch.h"
#include "UI/UILayer.h"
#include "Camera.h"
#include "Controls/AppKeyBindings.h"

#define OBJ_VIEWER_IS_Y_VECTOR_FLIPPED(X) std::cos(glm::radians(X)) < 0
#define ASPECT(X,Y) (float)X / (float)Y

constexpr float kZfar = 100.0f;
constexpr float kZnear = 0.1f;
constexpr float kFieldOfView = 45.0f;
constexpr glm::vec3 kCameraDefaultOrigin = glm::vec3{0,1.0f,0};

OBJ_Viewer::Camera::Camera(float CameraZoom, Size2D screenSize, Application& app):
	m_app(app),m_cameraCenter(kCameraDefaultOrigin)
{
	this->m_zoom = CameraZoom;
	m_projectionMatrix = glm::perspective(glm::radians(kFieldOfView), ASPECT(screenSize.width, screenSize.height), kZnear, kZfar);

	this->m_EulerAngles.m_pitchAngle = 0.0f;
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
	constexpr float kScrollSensitivity = 0.01f;
	constexpr float kMaxScrollZoom = 0.002;
	this->m_zoom += this->m_zoom - e.GetScrollPosition().y <= kMaxScrollZoom ? 0 : -e.GetScrollPosition().y;

	if (!m_isProjectionPerspective)
	{
		Size2D current_window_size = m_app.GetSceneViewport().GetViewportSize();
		CalculateOthoProjection(current_window_size);
	}

	RecalculateViewMatrix();
}

void OBJ_Viewer::Camera::onMousePositionChanged(MousePositionEvent& e)
{
	auto& current_mouse_position = e.GetMousePos();
#pragma region Check for LMB
	if (m_app.GetGlobalInputHandler().isMouseButtonPressed(MouseKey_kLeftMouseButton) &&
		m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		//We update the previous x and y position.
		m_EulerAngleHelper.calculateEulerAngles(current_mouse_position);
	}
	else if (m_app.GetGlobalInputHandler().isMouseButtonHeld(MouseKey_kLeftMouseButton) &&
		m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		m_EulerAngles += m_EulerAngleHelper.calculateEulerAngles(current_mouse_position);
		m_EulerAngleHelper.ConstrainAngles(m_EulerAngles);
		CalculatePositionVector();
	}
#pragma endregion

#pragma region  Check for RMB
	if (m_app.GetGlobalInputHandler().isMouseButtonPressed(MouseKey_kRightMouseButton) &&
		m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		//We update the previous x and y position.
		m_lastMousePos = current_mouse_position;
	}
	else if (m_app.GetGlobalInputHandler().isMouseButtonHeld(MouseKey_kRightMouseButton) &&
		m_app.GetGlobalInputHandler().GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		constexpr float kCameraShiftingSpeed = .005;
		glm::mat3 view_space_no_transform_matrix = glm::mat3(m_viewMatrix);
		glm::vec3 view_space_delta_vector(0);
		//We calculate the offset in camera/view space first.
		Position2D delta = Position2D{ current_mouse_position.x - m_lastMousePos.x,current_mouse_position.y - m_lastMousePos.y};
		view_space_delta_vector.x = delta.x* kCameraShiftingSpeed;
		//We flip the y since opengl flips its y component 
		view_space_delta_vector.y = -delta.y* kCameraShiftingSpeed;
		//TL;DR: We use transpose as a cheap inverse since the "viewSpaceOrthogonalVectors" is a 3 orthogonal vector matrix
		//and then we bring our viewSpace delta into word space.
		/*Why we do it like this ? 
		* To pan/shift a camera we wish to move the screen up or down problem is that up/down and right/left is different for the camera
		* since the camera Z axis is always the cameras front in view space we know that there is a plane perpendicular on the camera Z
		* axis that we can move. To find where we camera up/down and right/left in world space we have to find this plane. The best approach is to 
		* calculate the movement in view space and then inverse the view matrix to get what the vector will be in world space.
		*/
		view_space_delta_vector = glm::transpose(view_space_no_transform_matrix) * view_space_delta_vector;
		m_lastMousePos = current_mouse_position;
		m_cameraCenter += view_space_delta_vector;
		RecalculateViewMatrix();
	}
#pragma endregion

}

void OBJ_Viewer::Camera::onViewportChanged(SceneViewportResizeEvent& e)
{
	const Size2D window_size = e.GetViewportSize();
	RecalculateProjection(window_size);
}

void OBJ_Viewer::Camera::onKeyPressedEvent(KeyboardKeyEvent& e)
{
	if (e.GetKeyCode() == AppKeyBinding_::AppKeyBinding_kResetCameraPivot)
	{
		m_cameraCenter = kCameraDefaultOrigin;
		RecalculateViewMatrix();
	}
}

void OBJ_Viewer::Camera::RecalculateProjection(Size2D windowSize)
{
	windowSize = (windowSize.width == 0 || windowSize.height == 0) ? m_app.GetSceneViewport().GetViewportSize() : windowSize;

	if (m_isProjectionPerspective)
		m_projectionMatrix = glm::perspective(glm::radians(kFieldOfView), ASPECT(windowSize.width, windowSize.height) , kZnear, kZfar);
	else
		CalculateOthoProjection(windowSize);
}

void OBJ_Viewer::Camera::OnProjectionModeChanged(EventCameraProjectionChanged& e)
{
	m_isProjectionPerspective = e.isCameraProjectionPerspective();
	RecalculateProjection();
}

void OBJ_Viewer::Camera::CalculateOthoProjection(Size2D windowSize)
{
	constexpr float kOrthographicScaleFactor = 500.0f;

	float orthographic_scale = this->m_zoom / kOrthographicScaleFactor;
	/*Orthographic projection doesn't really have a zoom since all you see must stay true to its size so instead we shrink the
	* box defined by the left,right,bottom,top planes so that smaller values will get bigger meaning that since ortho matrix
	* is just a scale and translate we make the scale bigger by multiplying left,right,bottom,top with the 'orthoScale'.
	*/
	m_projectionMatrix = glm::ortho(-((float)windowSize.width / 2) * orthographic_scale, ((float)windowSize.width / 2) * orthographic_scale,
		-((float)windowSize.height / 2) * orthographic_scale, ((float)windowSize.height / 2) * orthographic_scale, -1.f/ orthographic_scale, kZfar);
}



void OBJ_Viewer::Camera::RecalculateViewMatrix()
{
	glm::vec3 up_vector(0, 1, 0);
	up_vector.y *= OBJ_VIEWER_IS_Y_VECTOR_FLIPPED(this->m_EulerAngles.m_pitchAngle) ? -1 : 1;
	m_viewMatrix = glm::lookAt(m_position * m_zoom + m_cameraCenter, m_cameraCenter, up_vector);
}

void OBJ_Viewer::Camera::OnEvent(Event& e)
{
	switch (e.GetEventType())
	{
	case EventType_kViewportSizeChanged:
		this->onViewportChanged(dynamic_cast<SceneViewportResizeEvent&>(e));
		break;
	case EventType_kMousePositionChanged:
		this->onMousePositionChanged(dynamic_cast<MousePositionEvent&>(e));
		break;
	case EventType_kMouseScrollChanged:
		this->onScrollChanged(dynamic_cast<ScrollPositionChanged&>(e));
		break;
	case EventType_kKeyPressed:
		this->onKeyPressedEvent(dynamic_cast<KeyboardKeyEvent&>(e));
		break;
	case EventType_kCameraProjectionChanged:
		this->OnProjectionModeChanged(dynamic_cast<EventCameraProjectionChanged&>(e));
		break;
	default:
		break;
	}
}


OBJ_Viewer::EulerAngles OBJ_Viewer::EulerAngleHelper::calculateEulerAngles(Position2D newMousePosition)
{
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
