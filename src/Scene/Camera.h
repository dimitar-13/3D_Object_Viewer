#pragma once
#include "pch.h"
#include "Core/Events.h"
#include "Controls/InputHandler.h"
#include "Core/WindowHandler.h"
#include "Core/AppEvent.h"
namespace OBJ_Viewer
{
	struct EulerAngles {
		float yawAngle;
		float pitchAngle;
		EulerAngles& operator+=(const EulerAngles& other) {
			this->yawAngle += other.yawAngle;
			this->pitchAngle += other.pitchAngle;
			return *this;
		}
		EulerAngles& operator-(const EulerAngles& other)
		{
			this->yawAngle -= other.yawAngle;
			this->pitchAngle -= other.pitchAngle;
			return *this;
		}
	};
	class EulerAngleHelper {
	public:
        EulerAngleHelper()
        {
            m_EulerAngles.pitchAngle = 0.0f;
            m_EulerAngles.yawAngle = 90.0f;
        }
        EulerAngles GetEulerAngles()const { return m_EulerAngles; }
        static EulerAngles ConvertMousePositionToAngles(Position2D position_to_convert);
        void IncreaseEulerAngles(const EulerAngles& angles) { m_EulerAngles += angles; ConstrainAngles(); }
    private:
        bool IsAngleWithinBounds(float angle) { return angle >= -360.0f && angle <= 360.0f; }
		void ConstrainAngles();
	private:
        EulerAngles m_EulerAngles {};
	};

	class Camera : public Listener
	{
	public:
		Camera(float CameraZoom,InputHandler& application_inputHandlerRef,const SceneViewport& kApplicationViewportManagerRef);
		void GetViewAndProjectionSeparate(glm::mat4& pView, glm::mat4& pProj)const { pView = m_viewMatrix; pProj = m_projectionMatrix; }
		glm::mat4 GetViewProjMatrix()const { return m_projectionMatrix * m_viewMatrix; }
		glm::mat4 GetViewProjNoTranslation()const { return m_projectionMatrix* glm::mat4(glm::mat3(m_viewMatrix)); };
		glm::vec3 GetCameraPos()const { return this->m_position; }
        bool IsCameraProjectionPerspective()const { return m_isProjectionPerspective; }
		void SetProjection(bool isProjectionPerspective = true) { m_isProjectionPerspective = isProjectionPerspective; RecalculateProjection(); }
	private:
        bool IsCameraYVectorFlipped(float pitch_angle);
        float GetAspectRatio(Size2D size) {return static_cast<float>(size.width) / static_cast<float>(size.height); }
		void RecalculateViewMatrix();
		void CalculatePositionVector();
#pragma region On Event
		void onScrollChanged(ScrollPositionChanged& e);
		void onMousePositionChanged(MousePositionEvent& e);
		void onViewportChanged(SceneViewportResizeEvent& e);
		void onKeyPressedEvent(KeyboardKeyEvent& e);
		void RecalculateProjection(Size2D windowSize = {0,0});
		void OnProjectionModeChanged(EventCameraProjectionChanged& e);
		void CalculateOthoProjection(Size2D windowSize);
#pragma endregion
	private:
		float m_zoom;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		EulerAngleHelper m_EulerAngleHelper;
		glm::vec3 m_position;
		InputHandler& m_applicationInputHandler;
        const SceneViewport& m_applicationViewportManager;
		glm::vec3 m_cameraCenter;
		Position2D m_lastMouseMovementPosition{};
        Position2D m_lastMouseShiftPosition{};
		bool m_isProjectionPerspective = true;
		// Inherited via Listener
		void OnEvent(Event& e) override;
	};
}

