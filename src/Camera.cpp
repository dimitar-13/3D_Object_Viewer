#include "Camera.h"

OBJ_Viewer::Camera::Camera(float CameraZoom, int width, int height)
{
	this->m_zoom = CameraZoom;
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)width / (float)height, 0.1f, 100.0f);
	//CameraZoom units from the origin or vec3(0,0,1) * CameraZoom;
	this->m_EulerAngles.m_pitchAngle =0.0f;
	this->m_EulerAngles.m_yawAngle = 90.0f;
	//Use to set the x_previous and y_previous of the 'm_EulerAngleHelper' so we can avoid conditions;
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

glm::mat4 OBJ_Viewer::Camera::GetViewProjMatrix()const
{
	return m_projectionMatrix * glm::lookAt(m_position *m_zoom,glm::vec3(0.0f), glm::vec3(0, 1, 0));
}

void OBJ_Viewer::Camera::Update(MessageType type, double xpos, double ypos)
{
	//Yay mouse position has updated :)
	if (type == MessageType::MOUSE_POSITION_CHANGED)
	{
	m_EulerAngles += m_EulerAngleHelper.calculateEulerAngles(xpos, ypos);
	CalculatePositionVector();
	}
	else if(type == MessageType::MOUSE_SCROLL_CHANGED)
	{
		//Calculate the new zoom lvl;
		//TODO:Add restrains so that user scroll to inifity;
		constexpr float sensitivity = 0.01f;
		this->m_zoom += -ypos ;
	}
}

void OBJ_Viewer::Camera::Update(MessageType type, int newWidth, int newHeight)
{
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)newWidth / (float)newHeight, 0.1f, 100.0f);
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
	//Check if mouse button is held and then move;
	EulerAngles result = {static_cast<float>(xpos - m_previousXPos), static_cast<float>(m_previousYPos - ypos)};
	m_previousXPos = xpos;
	m_previousYPos = ypos;
	return result;
}
