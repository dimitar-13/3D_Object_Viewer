#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"IObserver.h"
namespace OBJ_Viewer {
	struct WindowMetrics
	{
		int m_winWidth;
		int m_winHeight;
	};

	class Window {
	public:
		Window(GLFWwindow* glfwWindow, WindowMetrics windowMetrics, const char* winTitle)
			:m_glfwWindow(glfwWindow), m_windowMetrics(windowMetrics), m_winTitle(winTitle){}
		WindowMetrics GetWindowSize()const { return this->m_windowMetrics; }
		GLFWwindow* GetGLFW_Window()const { return this->m_glfwWindow; }
	private:
		WindowMetrics m_windowMetrics;
		GLFWwindow* m_glfwWindow;
		const char* m_winTitle;
	};
	class WindowMousePosChangedNotifier :INotifier {
		// Inherited via INotifier
	public:
		//TODO:Find a way to have this notifier in the window handler or in such a way that a observer can subscribe to it using the Window class.
		void Attach(IObserver* observer) override;
		void Detach(IObserver* observer) override;
		void Notify() override;
	};
	class WindowHandler {
	public:
		static Window* CreateAndBindWindow(WindowMetrics winMatrics, const char * winTitle);
	private:
		static void glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
	};
}
