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
		Window(WindowMetrics windowMetrics, const char* winTitle);
		WindowMetrics GetWindowSize()const { return this->m_windowMetrics; }
		GLFWwindow* GetGLFW_Window()const { return this->m_glfwWindow; }
		Notifier<double, double>& GetMousePosNotifier() { return m_posChangeNotifier; }
		Notifier<double, double>& GetScrollChangeNotifier() { return m_scrollChangeNotifier; }
		Notifier<int, int>& GetWindowSizeChangeNotifier() { return m_windowSizeChanged; }
		Notifier<int, int, int,int>* GetKeyNotifier() { return &m_windowKeyNotifier; }
		Notifier<int, int, int>* GetMouseButtonNotifier() { return &m_mouseButtonNotifier; }


	private:
		WindowMetrics m_windowMetrics;
		GLFWwindow* m_glfwWindow;
		const char* m_winTitle;
		Notifier<double, double> m_posChangeNotifier;
		Notifier<double, double> m_scrollChangeNotifier;
		Notifier<int, int> m_windowSizeChanged;
		/// <summary>
		///  int key, int action, int mods
		/// </summary>
		Notifier<int, int, int,int> m_windowKeyNotifier;
		Notifier<int, int, int> m_mouseButtonNotifier;

 	private:
		void SetWindowCallback();
		void glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
	};
}
