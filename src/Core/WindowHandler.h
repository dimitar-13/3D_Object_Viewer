#pragma once
#include "pch.h"
#include "Events.h"
#include "CommonAppData.h"
namespace OBJ_Viewer {

	enum WindowState
	{
		WINDOW_STATE_UKNOWN = 0,
		WINDOW_STATE_MINIMIZED,
		WINDOW_STATE_FULL_SCREEN,
		WINDOW_STATE_NORMAL
	};
	class Application;
	class Window {
	public:
		Window(Size2D windowMetrics, const char* winTitle);
		~Window() { glfwDestroyWindow(m_glfwWindow); }
		Size2D GetWindowSize()const { return this->m_windowSize; }
		GLFWwindow* GetGLFW_Window()const { return this->m_glfwWindow; }
		bool isWinContextInitialized() { return  m_glfwWindow != nullptr;}
	private:
		Size2D m_windowSize;
		GLFWwindow* m_glfwWindow = nullptr;
		const char* m_winTitle;
		std::function<void(Event&)> m_onEvent;
		WindowState windowState = WINDOW_STATE_NORMAL;
	private:
		
		friend class Application;
		void SetOnEventCallback(const std::function<void(Event&)>& onEventFunc){ m_onEvent = onEventFunc; }
 	private:
		void SetWindowCallback();
		void glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
	};
}
