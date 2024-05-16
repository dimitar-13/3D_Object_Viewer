#include "WindowHandler.h"
#include"WindowInputFuncs.h"
#include<functional>
#include"imgui.h"

OBJ_Viewer::Window::Window(WindowMetrics windowMetrics, const char* winTitle)
{
	this->m_winTitle = winTitle;
	this->m_windowMetrics = windowMetrics;
	GLFWwindow* glfwWindow = glfwCreateWindow(windowMetrics.m_winWidth, windowMetrics.m_winHeight, winTitle, NULL, NULL);
	glfwMakeContextCurrent(glfwWindow);
	this->m_glfwWindow = glfwWindow;
	SetWindowCallback();
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
}
void OBJ_Viewer::Window::glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	//Update the window information;
	m_posChangeNotifier.Notify(MOUSE_POSITION_CHANGED,xpos, ypos);
}

void OBJ_Viewer::Window::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void OBJ_Viewer::Window::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_scrollChangeNotifier.Notify(MOUSE_SCROLL_CHANGED,xoffset, yoffset);
}

void OBJ_Viewer::Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void OBJ_Viewer::Window::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	this->m_windowMetrics = { width,height };
	//glViewport(0, 0, width, height);
	m_windowSizeChanged.Notify(WINDOW_SIZE_CHANGED, width, height);
}
