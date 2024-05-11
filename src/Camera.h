#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
namespace OBJ_Viewer
{
	class Camera
	{
	public:
		Camera(glm::vec3 position, int width, int height);
		glm::mat4 GetViewProjMatrix();
		void recalculatePosition(float deltaMouseX, float deltaMouseY);
		//TODO:ADD functions for Euler angles of rotation.
	private:
		glm::vec3 m_position;
		glm::mat4 m_projectionMatrix;
		float m_pitchAngle;
		float m_yawAngle;
		glm::vec3 dirVec;

	};
}

