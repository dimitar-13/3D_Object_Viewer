#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"IObserver.h"
namespace OBJ_Viewer
{
	struct EulerAngles {
		float m_yawAngle;
		float m_pitchAngle;
		EulerAngles operator+=(EulerAngles other) {
			return { other.m_yawAngle + this->m_yawAngle, other.m_pitchAngle + this->m_pitchAngle};
		}
	};
	class EulerAngleHelper {
	public:
		EulerAngles calculateEulerAngles(double xpos, double ypos);
	private:
		double m_previousXPos;
		double m_previousYPos;
	};

	class Camera:
		public IObserver<double,double>,
			   IObserver<int,int>
	{
	public:
		Camera(float CameraZoom, int width, int height);
		void CalculatePositionVector();
		glm::mat4 GetViewProjMatrix();
	private:
		float m_zoom;
		glm::mat4 m_projectionMatrix;
		EulerAngles m_EulerAngles;
		EulerAngleHelper m_EulerAngleHelper;
		glm::vec3 m_position;

		// Inherited via IObserver
		void Update(double xpos, double ypos) override;
		void Update(int newWidht, int newHeight) override;
	};
}

