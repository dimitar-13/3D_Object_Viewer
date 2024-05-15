#include "Camera.h"

OBJ_Viewer::Camera::Camera(float CameraZoom, int width, int height)
{
	this->m_zoom = CameraZoom;
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)width / (float)height, 0.1f, 100.0f);
	this->m_EulerAngles.m_pitchAngle =0.0f;
	this->m_EulerAngles.m_yawAngle = 90.0f;
	m_EulerAngleHelper.calculateEulerAngles(this->m_EulerAngles.m_yawAngle,this->m_EulerAngles.m_pitchAngle);
	CalculatePositionVector();
}

void OBJ_Viewer::Camera::CalculatePositionVector()
{
	m_position.y = std::sin(glm::radians(this->m_EulerAngles.m_pitchAngle));
	m_position.z = std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) * std::sin(glm::radians(this->m_EulerAngles.m_yawAngle));
	m_position.x = std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) * std::cos(glm::radians(this->m_EulerAngles.m_yawAngle));
	m_position = glm::normalize(m_position);
}

glm::mat4 OBJ_Viewer::Camera::GetViewProjMatrix()
{
	return m_projectionMatrix * glm::lookAt(m_position *m_zoom,glm::vec3(0.0f), glm::vec3(0, 1, 0));
}

void OBJ_Viewer::Camera::Update(double xpos, double ypos)
{
	//Yay mouse position has updated :)
	EulerAngles test = m_EulerAngleHelper.calculateEulerAngles(xpos, ypos);
	m_EulerAngles.m_pitchAngle += test.m_pitchAngle;
	m_EulerAngles.m_yawAngle+= test.m_yawAngle;

	CalculatePositionVector();
}

void OBJ_Viewer::Camera::Update(int newWidht, int newHeight)
{
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)newWidht / (float)newHeight, 0.1f, 100.0f);
}

//void OBJ_Viewer::Camera::Update()
//{
//	//TODO:When the user presses button(button for reset) it will reset the camera to look at the starting position.
//	//TODO:When the user presses button(button for reset) it will reset the scroll to be at the starting value.
//
//	//(NOTE)In case of 2 updates one for the mouse and other for the button press how will this be made ? 2 different functions maybe.
//	 
//	//Get the new position from the window class.
//	//recalculatePosition();
//}

OBJ_Viewer::EulerAngles OBJ_Viewer::EulerAngleHelper::calculateEulerAngles(double xpos, double ypos)
{

	EulerAngles result = {static_cast<float>(xpos - m_previousXPos), static_cast<float>(m_previousYPos - ypos)};
	m_previousXPos = xpos;
	m_previousYPos = ypos;
	return result;
}
