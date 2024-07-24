#include "pch.h"
#include "WindowHandler.h"
#include"AppEvent.h"
OBJ_Viewer::Window::Window(Size2D windowMetrics, const char* winTitle, std::function<void(Event&)> onEventFunc)
{
	this->m_winTitle = winTitle;
	this->m_windowSize = windowMetrics;
	GLFWwindow* glfwWindow = glfwCreateWindow(windowMetrics.width, windowMetrics.height, winTitle, NULL, NULL);
	glfwMakeContextCurrent(glfwWindow);
	this->m_glfwWindow = glfwWindow;
	m_onEvent = onEventFunc;
	SetWindowCallback();
}

glm::mat4 OBJ_Viewer::Window::GetViewportMatrix()const
{
	glm::mat4 result(1);
	Size2D winSize = m_windowSize;
	result[0][0] = (float)winSize.width / 2;
	result[1][1] = (float)winSize.height / 2;

	result[2][0] = result[0][0];
	result[2][1] = result[1][1];

	return result;
}

void OBJ_Viewer::Window::SetWindowCallback()
{
	//TL;DR;
	//This is done because of the way OOP works.
	/*
	When you have a object lets say A and that object have member functions lets say foo.Every non-static member function has an invincible parameter
	that is the reference to the object A ,holding/owning the function.Might make more sense to think it as: if objects of classes can have different members/values
	different from one another and we can access those values via the object doesn't it make sense that for a class non-static function the word this is the reference itself.
	Hope I didn't make this confusing if someone reads this comment.
	*/
	glfwSetWindowUserPointer(m_glfwWindow, reinterpret_cast<void*>(this));
	auto CursorPosCallback = [](GLFWwindow* window, double xpos, double ypos)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->glfwCursorPositionCallback(window, xpos, ypos);
		};
	glfwSetCursorPosCallback(m_glfwWindow, CursorPosCallback);

	auto SizeCallback = [](GLFWwindow* window, int newWidth, int newHeight)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->glfwWindowSizeCallback(window, newWidth, newHeight);
		};
	glfwSetWindowSizeCallback(m_glfwWindow, SizeCallback);

	auto MouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->glfwMouseButtonCallback(window, button, action, mods);
		};
	glfwSetMouseButtonCallback(m_glfwWindow, MouseButtonCallback);

	auto ScrollCallback = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->glfwScrollCallback(window, xoffset, yoffset);
		};
	glfwSetScrollCallback(m_glfwWindow, ScrollCallback);

	auto buttonCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->glfwKeyCallback(window, key, scancode, action, mods);
		};
	glfwSetKeyCallback(m_glfwWindow, buttonCallback);

}
void OBJ_Viewer::Window::glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	MousePositionEvent mouseEvent({ xpos, ypos });
	m_onEvent(mouseEvent);
}

void OBJ_Viewer::Window::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	MouseKeyEvent mouseKeyStateChangeEvent(static_cast<MouseKey>(button), action, mods);
	m_onEvent(mouseKeyStateChangeEvent);
}

void OBJ_Viewer::Window::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ScrollPositionChanged scrollPositionChangedEvent({ xoffset, yoffset });
	m_onEvent(scrollPositionChangedEvent);
}

void OBJ_Viewer::Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyboardKeyEvent keyboardKeyStateEvent(static_cast<KeyboardKey>(key), scancode, action, mods);
	m_onEvent(keyboardKeyStateEvent);
}

void OBJ_Viewer::Window::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	int isVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE);
	this->m_windowSize = Size2D{ width,height };
	if (width == 0 || height == 0)
	{
		WindowStateChangedEvent e(Size2D{ width, height }, WINDOW_STATE_MINIMIZED);
		m_onEvent(e);
		return;
	}
	else if (m_windowSize.width == 0 || m_windowSize.height == 0 && (width != 0 || height != 0))
	{
		WindowStateChangedEvent e(Size2D{ width, height }, WINDOW_STATE_NORMAL);
		m_onEvent(e);
		return;
	}
	WindowResizeEvent e(Size2D{ width, height });
	m_onEvent(e);
}

