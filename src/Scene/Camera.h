#pragma once
#include "pch.h"
#include"Events.h"
#include"InputHandler.h"
#include"WindowHandler.h"
#include"AppEvent.h"
namespace OBJ_Viewer
{
	struct EulerAngles {
		float m_yawAngle;
		float m_pitchAngle;
		void operator+=(EulerAngles other) {
			this->m_yawAngle += other.m_yawAngle;
			this->m_pitchAngle += other.m_pitchAngle;
		}
		EulerAngles operator-(EulerAngles other)
		{
			return{this->m_yawAngle -= other.m_yawAngle,
				   this->m_pitchAngle -= other.m_pitchAngle};
		}
	};
	class EulerAngleHelper {
	public:
		EulerAngles calculateEulerAngles(Position2D newMousePosition);
		void ConstrainAngles(EulerAngles& angle);
	private:
		Position2D m_previousMousePosition;
	};

	class Camera : public Listener
	{
	public:
		Camera(float CameraZoom,Size2D screenSize, Application& app);
		Camera(const Camera& other) = delete;
		void CalculatePositionVector();
		//void GetViewAndProjectionSeparate(glm::mat4* pView, glm::mat4* pProj) { pView = &m_viewMatrix; pProj = &m_projectionMatrix; }
		void GetViewAndProjectionSeparate(glm::mat4* pView, glm::mat4* pProj)const { *pView = m_viewMatrix; *pProj = m_projectionMatrix; }
		glm::mat4 GetViewProjMatrix()const { return m_projectionMatrix * m_viewMatrix; }
		glm::mat4 GetViewProjNoTranslation()const { return m_projectionMatrix* glm::mat4(glm::mat3(m_viewMatrix)); };
		glm::vec3 GetCameraPos()const { return this->m_position; }
		void SetProjection(bool isProjectionPerspective = true) { m_isProjectionPerspective = isProjectionPerspective; RecalculateProjection(); }
	private:
		void RecalculateViewMatrix();

		void onScrollChanged(ScrollPositionChanged& e);
		void onMousePositionChanged(MousePositionEvent& e);
		void onWinSizeChanged(SceneViewportResizeEvent& e);
		void onKeyPressedEvent(KeyboardKeyEvent& e);
		void RecalculateProjection(Size2D windowSize = {0,0});
		void OnProjectionModeChanged(EventCameraProjectionChanged& e);
		void CalculateOthoProjection(Size2D windowSize);
	private:
		float m_zoom;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		EulerAngles m_EulerAngles;
		EulerAngleHelper m_EulerAngleHelper;
		glm::vec3 m_position;
		Application& m_app;

		glm::vec3 m_cameraCenter = glm::vec3(0);
		Position2D m_lastMousePos = Position2D{0,0};

		bool m_isProjectionPerspective = true;
		// Inherited via Listener
		void OnEvent(Event& e) override;
	};
}

