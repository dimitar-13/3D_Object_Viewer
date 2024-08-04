#include "pch.h"
#include "UI/UILayer.h"
#include "Camera.h"
#include "Controls/AppKeyBindings.h"

constexpr float kZfar = 100.0f;
constexpr float kZnear = 0.1f;
constexpr float kFieldOfView = 45.0f;
constexpr glm::vec3 kCameraDefaultOrigin = glm::vec3{0,1.0f,0};

OBJ_Viewer::Camera::Camera(float camera_zoom,
    InputHandler& application_inputHandlerRef, const SceneViewport& kApplicationViewportManagerRef):

    m_applicationInputHandler(application_inputHandlerRef),
    m_applicationViewportManager(kApplicationViewportManagerRef),
    m_cameraCenter(kCameraDefaultOrigin),
    m_zoom(camera_zoom)
{
    Size2D viewport_size = m_applicationViewportManager.GetViewportSize();
	m_projectionMatrix = glm::perspective(glm::radians(kFieldOfView), GetAspectRatio(viewport_size), kZnear, kZfar);

	//Use to set the x_previous and y_previous of the 'm_EulerAngleHelper' so we can avoid conditions;
	CalculatePositionVector();

}

void OBJ_Viewer::Camera::CalculatePositionVector()
{
    const float kPitchAngleInRadians = glm::radians(this->m_EulerAngleHelper.GetEulerAngles().pitchAngle);
    const float kNegativeYawAngleInRadians = glm::radians(-this->m_EulerAngleHelper.GetEulerAngles().yawAngle);

	m_position.y = std::sin(kPitchAngleInRadians);
	m_position.z = std::cos(kPitchAngleInRadians) * std::sin(kNegativeYawAngleInRadians);
	m_position.x = std::cos(kPitchAngleInRadians) * std::cos(kNegativeYawAngleInRadians);
	m_position = glm::normalize(m_position);
	RecalculateViewMatrix();
}

void OBJ_Viewer::Camera::onScrollChanged(ScrollPositionChanged& e)
{
    constexpr float kMinScrollZoom = 0.002f;
    constexpr float kMaxScrollZoom = 20.0f;
    constexpr float kScrollRange = kMaxScrollZoom - kMinScrollZoom;
    constexpr float kAdjustableScrollSensitivityBias = .05f;

    float scroll_delta = this->m_zoom - e.GetScrollPosition().y;
    const float kAdjustableScrollSensitivity = scroll_delta/ kScrollRange;

    float get_ranged_scroll_amount = scroll_delta <= kMinScrollZoom ||
        scroll_delta >= kMaxScrollZoom ? 0 : -e.GetScrollPosition().y;

    this->m_zoom += get_ranged_scroll_amount * (kAdjustableScrollSensitivity + kAdjustableScrollSensitivityBias);

	if (!m_isProjectionPerspective)
	{
		Size2D current_window_size = m_applicationViewportManager.GetViewportSize();
		CalculateOthoProjection(current_window_size);
	}

	RecalculateViewMatrix();
}

void OBJ_Viewer::Camera::onMousePositionChanged(MousePositionEvent& e)
{
	auto& current_mouse_position = e.GetMousePos();
#pragma region Check for LMB
	if (m_applicationInputHandler.isMouseButtonPressed(static_cast<MouseKey_>(AppKeyBinding_kCameraRotation)) &&
		m_applicationInputHandler.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
        m_lastMouseMovementPosition = current_mouse_position;
	}
    else if (m_applicationInputHandler.isMouseButtonHeld(static_cast<MouseKey_>(AppKeyBinding_kCameraRotation)) &&
        m_applicationInputHandler.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
    {
        Position2D mouse_delta = { current_mouse_position.x - m_lastMouseMovementPosition.x,
            current_mouse_position.y - m_lastMouseMovementPosition.y };
       
        m_lastMouseMovementPosition = current_mouse_position;

        m_EulerAngleHelper.IncreaseEulerAngles(EulerAngleHelper::ConvertMousePositionToAngles(mouse_delta));

		CalculatePositionVector();
	}
#pragma endregion

#pragma region  Check for RMB
	if (m_applicationInputHandler.isMouseButtonPressed(static_cast<MouseKey_>(AppKeyBinding_kCameraShifting)) &&
		m_applicationInputHandler.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		//We update the previous x and y position.
		m_lastMouseShiftPosition = current_mouse_position;
	}
	else if (m_applicationInputHandler.isMouseButtonHeld(static_cast<MouseKey_>(AppKeyBinding_kCameraShifting)) &&
             m_applicationInputHandler.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		constexpr float kCameraShiftingSpeed = .005;
		glm::mat3 view_space_no_transform_matrix = glm::mat3(m_viewMatrix);
		glm::vec3 view_space_delta_vector(0);
		//We calculate the offset in camera/view space first.
		Position2D delta = Position2D{ current_mouse_position.x - m_lastMouseShiftPosition.x,current_mouse_position.y - m_lastMouseShiftPosition.y};
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
        m_lastMouseShiftPosition = current_mouse_position;
		m_cameraCenter += view_space_delta_vector;
		RecalculateViewMatrix();
	}
#pragma endregion

}

void OBJ_Viewer::Camera::onViewportChanged(SceneViewportResizeEvent& e)
{
	const Size2D viewport_size = e.GetViewportSize();
	RecalculateProjection(viewport_size);
}

void OBJ_Viewer::Camera::onKeyPressedEvent(KeyboardKeyEvent& e)
{
	if (e.GetKeyCode() == AppKeyBinding_::AppKeyBinding_kResetCameraPivot)
	{
		m_cameraCenter = kCameraDefaultOrigin;
		RecalculateViewMatrix();
	}
}

void OBJ_Viewer::Camera::RecalculateProjection(Size2D viewport_size)
{
    viewport_size = (viewport_size.width == 0 || viewport_size.height == 0) ? m_applicationViewportManager.GetViewportSize() : viewport_size;

	if (m_isProjectionPerspective)
		m_projectionMatrix = glm::perspective(glm::radians(kFieldOfView), GetAspectRatio(viewport_size) , kZnear, kZfar);
	else
		CalculateOthoProjection(viewport_size);
}

void OBJ_Viewer::Camera::OnProjectionModeChanged(EventCameraProjectionChanged& e)
{
	m_isProjectionPerspective = e.isCameraProjectionPerspective();
	RecalculateProjection();
}

void OBJ_Viewer::Camera::CalculateOthoProjection(Size2D viewport_size)
{
	constexpr float kOrthographicScaleFactor = 500.0f;

	float orthographic_scale = this->m_zoom / kOrthographicScaleFactor;
	/*Orthographic projection doesn't really have a zoom since all you see must stay true to its size so instead we shrink the
	* box defined by the left,right,bottom,top planes so that smaller values will get bigger meaning that since ortho matrix
	* is just a scale and translate we make the scale bigger by multiplying left,right,bottom,top with the 'orthoScale'.
	*/
	m_projectionMatrix = glm::ortho(-((float)viewport_size.width / 2) * orthographic_scale, ((float)viewport_size.width / 2) * orthographic_scale,
		-((float)viewport_size.height / 2) * orthographic_scale, ((float)viewport_size.height / 2) * orthographic_scale, -1.f/ orthographic_scale, kZfar);
}

bool OBJ_Viewer::Camera::IsCameraYVectorFlipped(float pitch_angle)
{
   // We check the current angle but because of floating point errors when current_angle_in_radiance = +/- 90 or +/- 270
   // the cos function doesn't return 0 and leads to weird snapping when perfectly perpendicular
   // that's why we check the next angle and decide if our vector is indeed flipped.

   const float kNextAngleSignedOffsetBias = pitch_angle < 0 ? -1.0f : 1.0f;
   float current_angle_in_radiance = glm::radians(pitch_angle);
   float next_angle_in_radiance = glm::radians(pitch_angle + kNextAngleSignedOffsetBias);

   return std::cos(current_angle_in_radiance) < 0 && std::cos(next_angle_in_radiance) < 0;
}

void OBJ_Viewer::Camera::RecalculateViewMatrix()
{
	glm::vec3 up_vector(0, 1, 0);
    up_vector.y *= IsCameraYVectorFlipped(this->m_EulerAngleHelper.GetEulerAngles().pitchAngle) ? -1 : 1;
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


OBJ_Viewer::EulerAngles OBJ_Viewer::EulerAngleHelper::ConvertMousePositionToAngles(Position2D position_to_convert)
{
    return EulerAngles{static_cast<float>(position_to_convert.x), static_cast<float>(position_to_convert.y)};
}

void OBJ_Viewer::EulerAngleHelper::ConstrainAngles()
{
	char oposite_angle_sign = this->m_EulerAngles.pitchAngle > 0 ? -1 : 1;
    this->m_EulerAngles.pitchAngle += !IsAngleWithinBounds(this->m_EulerAngles.pitchAngle) ? oposite_angle_sign * 360.0f : 0;
	oposite_angle_sign = this->m_EulerAngles.yawAngle > 0 ? -1 : 1;
    this->m_EulerAngles.yawAngle += !IsAngleWithinBounds(this->m_EulerAngles.yawAngle) ? oposite_angle_sign * 360.0f : 0;
}
