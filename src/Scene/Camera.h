#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Events.h"
#include"InputHandler.h"
#include"WindowHandler.h"
#include<memory>
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

	//TODO:When the user presses button(button for reset) it will reset the camera to look at the starting position.
	//TODO:When the user presses button(button for reset) it will reset the scroll to be at the starting value.
	class Camera : public Listener
	{
	public:
		Camera(float CameraZoom,Size2D screenSize, Application& app);
		void CalculatePositionVector();
		//void GetViewAndProjectionSeparate(glm::mat4* pView, glm::mat4* pProj) { pView = &m_viewMatrix; pProj = &m_projectionMatrix; }
		void GetViewAndProjectionSeparate(glm::mat4* pView, glm::mat4* pProj)const { *pView = m_viewMatrix; *pProj = m_projectionMatrix; }
		glm::mat4 GetViewProjMatrix()const { return m_projectionMatrix * m_viewMatrix; }
		glm::mat4 GetViewProjNoTranslation()const { return m_projectionMatrix* glm::mat4(glm::mat3(m_viewMatrix)); };
		glm::vec3 GetCameraPos()const { return this->m_position; }	
	private:
		void RecalculateViewMatrix();

		void onScrollChanged(ScrollPositionChanged& e);
		void onMousePositionChanged(MousePositionEvent& e);
		void onWinSizeChanged(WindowResizeEvent& e);
		void onKeyPressedEvent(KeyboardKeyEvent& e);
	private:
		float m_zoom;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		EulerAngles m_EulerAngles;
		EulerAngleHelper m_EulerAngleHelper;
		glm::vec3 m_position;
		Application& app;

		glm::vec3 m_cameraCenter = glm::vec3(0);
		Position2D m_lastMousePos = Position2D{0,0};

		// Inherited via Listener
		void OnEvent(Event& e) override;
	};
}

