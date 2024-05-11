#include "Camera.h"

OBJ_Viewer::Camera::Camera(glm::vec3 position,int width,int height)
{
	this->m_position = position;
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)width / (float)height, 0.1f, 100.0f);
	this->m_pitchAngle =0.0f;
	this->m_yawAngle = -90.0f;

	dirVec.y = std::sin(glm::radians(m_pitchAngle));
	dirVec.z = std::cos(glm::radians(m_pitchAngle)) * std::sin(glm::radians(m_yawAngle));
	dirVec.x = std::cos(glm::radians(m_pitchAngle)) * std::cos(glm::radians(m_yawAngle));
	dirVec = glm::normalize(dirVec);
}

glm::mat4 OBJ_Viewer::Camera::GetViewProjMatrix()
{
	return m_projectionMatrix * glm::lookAt(dirVec * glm::length(this->m_position), glm::vec3(0.0f), glm::vec3(0, 1, 0));
}

void OBJ_Viewer::Camera::recalculatePosition(float deltaMouseX, float deltaMouseY)
{
	m_pitchAngle += deltaMouseY;
	m_yawAngle += deltaMouseX;
}

