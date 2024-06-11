#include "UI/UILayer.h"
#include "Camera.h"

OBJ_Viewer::Camera::Camera(float CameraZoom, int width, int height, InputHandler* pInputHandler)
{
	this->m_zoom = CameraZoom;
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)width / (float)height, 0.1f, 100.0f);
	//CameraZoom units from the origin or vec3(0,0,1) * CameraZoom;
	this->m_EulerAngles.m_pitchAngle =0.0f;
	this->m_EulerAngles.m_yawAngle = 90.0f;
	//Use to set the x_previous and y_previous of the 'm_EulerAngleHelper' so we can avoid conditions;
	m_EulerAngleHelper.calculateEulerAngles(this->m_EulerAngles.m_yawAngle,this->m_EulerAngles.m_pitchAngle);
	CalculatePositionVector();
	this->m_pInputHandler = pInputHandler;
}

void OBJ_Viewer::Camera::CalculatePositionVector()
{
	m_position.y = std::sin(glm::radians(this->m_EulerAngles.m_pitchAngle));
	m_position.z = std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) * std::sin(glm::radians(-this->m_EulerAngles.m_yawAngle));
	m_position.x = std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) * std::cos(glm::radians(-this->m_EulerAngles.m_yawAngle));
	m_position = glm::normalize(m_position);
}

void OBJ_Viewer::Camera::GetViewAndProjectionSeparate(glm::mat4* pView, glm::mat4* pProj)const
{
	glm::vec3 upVec(0, 1, 0);
	upVec.y *= std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) < 0 ? -1 : 1;
	*pView = glm::lookAt(m_position * m_zoom, glm::vec3(0.0f), upVec);
	*pProj = m_projectionMatrix;
}

glm::mat4 OBJ_Viewer::Camera::GetViewProjMatrix()const
{
	glm::vec3 upVec(0, 1, 0);
	upVec.y *= std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) < 0 ? -1 : 1;
	return m_projectionMatrix * glm::lookAt(m_position *m_zoom,glm::vec3(0.0f), upVec);
}

glm::mat4 OBJ_Viewer::Camera::GetViewProjNoTranslation() const
{
	glm::vec3 upVec(0, 1, 0);
	upVec.y *= std::cos(glm::radians(this->m_EulerAngles.m_pitchAngle)) < 0 ? -1 : 1;
	return m_projectionMatrix * glm::mat4(glm::mat3(glm::lookAt(m_position * m_zoom, glm::vec3(0.0f), upVec)));
}

void OBJ_Viewer::Camera::Update(MessageType type, double xpos, double ypos)
{
	//Yay mouse position has updated :)
	if (type == MessageType::MOUSE_POSITION_CHANGED)
	{
		if(m_pInputHandler->isMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && m_pInputHandler->GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
		{
			//We update the previous x and y position.
			m_EulerAngleHelper.calculateEulerAngles(xpos, ypos);
		}
		else if (m_pInputHandler->isMouseButtonHeld(GLFW_MOUSE_BUTTON_1) && m_pInputHandler->GetCurrentlyFocusedWindow() == UI_LAYER_SCENE_WINDOW_NAME)
		{
			m_EulerAngles += m_EulerAngleHelper.calculateEulerAngles(xpos, ypos);
			m_EulerAngleHelper.ConstrainAngles(m_EulerAngles);
			CalculatePositionVector();
		}
	}
	else if(type == MessageType::MOUSE_SCROLL_CHANGED)
	{
		//Calculate the new zoom lvl;
		//TODO:Add restrains so that user scroll to inifity;
		constexpr float sensitivity = 0.01f;
		constexpr float maxZoom = 0.002;
		this->m_zoom += this->m_zoom - ypos <= maxZoom ? 0 : -ypos;
	}
}

void OBJ_Viewer::Camera::Update(MessageType type, int newWidth, int newHeight)
{
	m_projectionMatrix = glm::perspective(std::cos(90.0f), (float)newWidth / (float)newHeight, 0.1f, 100.0f);
}

OBJ_Viewer::EulerAngles OBJ_Viewer::EulerAngleHelper::calculateEulerAngles(double xpos, double ypos)
{
	//Check if mouse button is held and then move;
	EulerAngles result = {static_cast<float>(xpos - m_previousXPos), static_cast<float>(m_previousYPos - ypos)};
	m_previousXPos = xpos;
	m_previousYPos = ypos;
	ConstrainAngles(result);
	return result;
}

void OBJ_Viewer::EulerAngleHelper::ConstrainAngles(EulerAngles& angle)
{
	char sing = angle.m_pitchAngle > 0 ? -1 : 1;
	angle.m_pitchAngle += !(angle.m_pitchAngle >= -360.0f && angle.m_pitchAngle <= 360.0f) ? sing * 360.0f : 0;
	sing = angle.m_yawAngle > 0 ? -1 : 1;
	angle.m_yawAngle += !(angle.m_yawAngle >= -360.0f && angle.m_yawAngle <= 360.0f) ? sing * 360.0f : 0;
}
