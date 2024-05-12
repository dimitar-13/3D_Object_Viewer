#include "WindowHandler.h"
#include"WindowInputFuncs.h"
#include<functional>

OBJ_Viewer::WindowMousePosChangedNotifier m_posNotifier;

OBJ_Viewer::Window* OBJ_Viewer::WindowHandler::CreateAndBindWindow(WindowMetrics winMetrics, const char* winTitle)
{
	GLFWwindow* glfwWindow = glfwCreateWindow(winMetrics.m_winWidth, winMetrics.m_winHeight, winTitle, NULL, NULL);
	glfwMakeContextCurrent(glfwWindow);

	glfwSetWindowSizeCallback(glfwWindow, WindowHandler::glfwWindowSizeCallback);
	glfwSetCursorPosCallback(glfwWindow, WindowHandler::glfwCursorPositionCallback);
	glfwSetMouseButtonCallback(glfwWindow, WindowHandler::glfwMouseButtonCallback);
	glfwSetScrollCallback(glfwWindow, WindowHandler::glfwScrollCallback);

	if (glfwWindow == NULL)
		return new Window(nullptr,{-1,-1},nullptr);

	return new Window(glfwWindow,winMetrics, winTitle);
}

void OBJ_Viewer::WindowHandler::glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_posNotifier.Notify();
}

void OBJ_Viewer::WindowHandler::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void OBJ_Viewer::WindowHandler::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void OBJ_Viewer::WindowHandler::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void OBJ_Viewer::WindowHandler::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
}


//OBJ_Viewer::WindowHandler::~WindowHandler()
//{
//	//Fix:Should be destroyed before the main app class;
//	glfwDestroyWindow(this->m_appWindow);
//}

void OBJ_Viewer::WindowMousePosChangedNotifier::Attach(IObserver* observer)
{
}

void OBJ_Viewer::WindowMousePosChangedNotifier::Detach(IObserver* observer)
{
}

void OBJ_Viewer::WindowMousePosChangedNotifier::Notify()
{
}
