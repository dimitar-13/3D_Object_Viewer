#pragma once
#include<glm/glm.hpp>
namespace OBJ_Viewer
{
	class Camera
	{
	public:
		Camera();
		glm::mat4 GetViewProjMatrix();
		//TODO:ADD functions for Euler angles of rotation.
	private:
		glm::vec3 m_position;
	};
}

