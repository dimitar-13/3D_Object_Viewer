#include "pch.h"
#include "UI/UILayer.h"
#include "Camera.h"
#include "Controls/AppKeyBindings.h"
#include <glm/gtc/quaternion.hpp>

constexpr float kZfar = 100.0f;
constexpr float kZnear = 0.1f;
constexpr float kFieldOfView = 45.0f;
constexpr float kDefaultCameraZoom = 5.0f;
constexpr glm::vec3 kCameraDefaultOrigin = glm::vec3{0,1.0f,0};

OBJ_Viewer::Camera::Camera(InputHandler& application_inputHandlerRef, const SceneViewport& kApplicationViewportManagerRef):

    m_ApplicationInputHandlerRef(application_inputHandlerRef),
    m_ApplicationSceneViewportRef(kApplicationViewportManagerRef),
    m_CameraLookAtPivot(kCameraDefaultOrigin),
    m_CurrentCameraZoomAmount(kDefaultCameraZoom)
{
    Size2D viewport_size = m_ApplicationSceneViewportRef.GetViewportSize();
	m_ProjectionMatrix = glm::perspective(glm::radians(kFieldOfView), GetAspectRatio(viewport_size), kZnear, kZfar);

	//Use to set the x_previous and y_previous of the 'm_EulerAngleHelper' so we can avoid conditions;
	CalculatePositionVector();

}

void OBJ_Viewer::Camera::CalculatePositionVector()
{
    const float kPitchAngleInRadians = glm::radians(this->m_EulerAngleHelper.GetEulerAngles().pitchAngle);
    const float kNegativeYawAngleInRadians = glm::radians(-this->m_EulerAngleHelper.GetEulerAngles().yawAngle);

	m_CurrentCameraPosition.y = std::sin(kPitchAngleInRadians);
	m_CurrentCameraPosition.z = std::cos(kPitchAngleInRadians) * std::sin(kNegativeYawAngleInRadians);
	m_CurrentCameraPosition.x = std::cos(kPitchAngleInRadians) * std::cos(kNegativeYawAngleInRadians);
	m_CurrentCameraPosition = glm::normalize(m_CurrentCameraPosition);
	CalculateViewMatrix();
}

void OBJ_Viewer::Camera::onScrollChanged(ScrollPositionChanged& e)
{
    constexpr float kMinScrollZoom = 0.15f;
    constexpr float kMaxScrollZoom = 20.0f;
    constexpr float kScrollRange = kMaxScrollZoom - kMinScrollZoom;
    constexpr float kAdjustableScrollSensitivityBias = .05f;

    float scroll_delta = this->m_CurrentCameraZoomAmount - e.GetScrollPosition().y;
    const float kAdjustableScrollSensitivity = scroll_delta/ kScrollRange;

    float get_ranged_scroll_amount = scroll_delta <= kMinScrollZoom ||
        scroll_delta >= kMaxScrollZoom ? 0 : -e.GetScrollPosition().y;

    this->m_CurrentCameraZoomAmount += get_ranged_scroll_amount * (kAdjustableScrollSensitivity + kAdjustableScrollSensitivityBias);

	if (!m_IsProjectionPerspective)
	{
		Size2D current_window_size = m_ApplicationSceneViewportRef.GetViewportSize();
		CalculateOthoProjection(current_window_size);
	}

	CalculateViewMatrix();
}

void OBJ_Viewer::Camera::onMousePositionChanged(MousePositionEvent& e)
{
	auto& current_mouse_position = e.GetMousePos();
#pragma region Check for LMB
	if (m_ApplicationInputHandlerRef.IsMouseButtonPressed(static_cast<MouseKey_>(AppKeyBinding_kCameraRotation)) &&
		m_ApplicationInputHandlerRef.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
        m_lastMouseMovementPosition = current_mouse_position;
	}
    else if (m_ApplicationInputHandlerRef.IsMouseButtonHeld(static_cast<MouseKey_>(AppKeyBinding_kCameraRotation)) &&
        m_ApplicationInputHandlerRef.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
    {
        Position2D mouse_delta = { current_mouse_position.x - m_lastMouseMovementPosition.x,
            current_mouse_position.y - m_lastMouseMovementPosition.y };
       
        m_lastMouseMovementPosition = current_mouse_position;
        m_EulerAngleHelper.IncreaseEulerAngles(EulerAngleHelper::ConvertMousePositionToAngles(mouse_delta));

		CalculatePositionVector();
	}
#pragma endregion

#pragma region  Check for RMB
	if (m_ApplicationInputHandlerRef.IsMouseButtonPressed(static_cast<MouseKey_>(AppKeyBinding_kCameraShifting)) &&
		m_ApplicationInputHandlerRef.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		//We update the previous x and y position.
		m_lastMouseShiftPosition = current_mouse_position;
	}
	else if (m_ApplicationInputHandlerRef.IsMouseButtonHeld(static_cast<MouseKey_>(AppKeyBinding_kCameraShifting)) &&
             m_ApplicationInputHandlerRef.GetCurrentlyFocusedWindow() == APP_FOCUS_REGIONS::kUI_SceneWindowName)
	{
		m_CameraLookAtPivot += CalculateMouseOffsetInWorldSpace(m_lastMouseShiftPosition, current_mouse_position);
        m_lastMouseShiftPosition = current_mouse_position;
		CalculateViewMatrix();
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
		m_CameraLookAtPivot = kCameraDefaultOrigin;
		CalculateViewMatrix();
	}
}

void OBJ_Viewer::Camera::RecalculateProjection(Size2D viewport_size)
{
    viewport_size = (viewport_size.width == 0 || viewport_size.height == 0) ? m_ApplicationSceneViewportRef.GetViewportSize() : viewport_size;

	if (m_IsProjectionPerspective)
		m_ProjectionMatrix = glm::perspective(glm::radians(kFieldOfView), GetAspectRatio(viewport_size) , kZnear, kZfar);
	else
		CalculateOthoProjection(viewport_size);
}

glm::vec3 OBJ_Viewer::Camera::CalculateMouseOffsetInWorldSpace(const Position2D& previous_mouse_position,const Position2D& current_mouse_position)
{
    constexpr float kCameraShiftingSpeed = .005;
    glm::mat3 view_space_no_transform_matrix = glm::mat3(m_ViewMatrix);
    glm::vec3 view_space_delta_vector(0);
    //We calculate the offset in camera/view space first.
    Position2D delta = Position2D{ current_mouse_position.x - previous_mouse_position.x,current_mouse_position.y - previous_mouse_position.y };
    view_space_delta_vector.x = delta.x * kCameraShiftingSpeed;
    //We flip the y since OpenGL flips its y component 
    view_space_delta_vector.y = -delta.y * kCameraShiftingSpeed;
    //TL;DR: We use transpose as a cheap inverse since the "viewSpaceOrthogonalVectors" is a 3 orthogonal vector matrix
    //and then we bring our viewSpace delta into word space.
    /*Why we do it like this ?
    * To pan/shift a camera we wish to move the screen up or down problem is that up/down and right/left is different for the camera
    * since the camera Z axis is always the cameras front in view space we know that there is a plane perpendicular on the camera Z
    * axis that we can move. To find where we camera up/down and right/left in world space we have to find this plane. The best approach is to
    * calculate the movement in view space and then inverse the view matrix to get what the vector will be in world space.
    */
    return glm::transpose(view_space_no_transform_matrix) * view_space_delta_vector;
}

void OBJ_Viewer::Camera::OnProjectionModeChanged(EventCameraProjectionChanged& e)
{
	m_IsProjectionPerspective = e.isCameraProjectionPerspective();
	RecalculateProjection();
}

void OBJ_Viewer::Camera::CalculateOthoProjection(Size2D viewport_size)
{
	constexpr float kOrthographicScaleFactor = 500.0f;

	float orthographic_scale = this->m_CurrentCameraZoomAmount / kOrthographicScaleFactor;
	/*Orthographic projection doesn't really have a zoom since all you see must stay true to its size so instead we shrink the
	* box defined by the left,right,bottom,top planes so that smaller values will get bigger meaning that since ortho matrix
	* is just a scale and translate we make the scale bigger by multiplying left,right,bottom,top with the 'orthoScale'.
	*/
	m_ProjectionMatrix = glm::ortho(-((float)viewport_size.width / 2) * orthographic_scale, ((float)viewport_size.width / 2) * orthographic_scale,
		-((float)viewport_size.height / 2) * orthographic_scale, ((float)viewport_size.height / 2) * orthographic_scale, -1.f/ orthographic_scale, kZfar);
}

bool OBJ_Viewer::Camera::IsCameraYVectorFlipped(float pitch_angle)
{
   float current_angle_in_radiance = glm::radians(pitch_angle);
   return std::cos(current_angle_in_radiance) < 0;
}

void OBJ_Viewer::Camera::CalculateViewMatrix()
{
	glm::vec3 up_vector(0, 1, 0);
    up_vector.y *= IsCameraYVectorFlipped(this->m_EulerAngleHelper.GetEulerAngles().pitchAngle) ? -1 : 1;
	m_ViewMatrix = glm::lookAt(m_CurrentCameraPosition*m_CurrentCameraZoomAmount,glm::vec3(0,0,0), up_vector);
    m_ViewMatrix = glm::translate(m_ViewMatrix,-m_CameraLookAtPivot);
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
