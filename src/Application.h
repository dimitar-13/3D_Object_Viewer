#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"Renderer.h"

namespace OBJ_Viewer {
	struct WindowMetrics
	{
		int m_winWidth;
		int m_winHeight;
	};
	class Application
	{
	public:
		static bool Init(int winWidth = 1000, int winHeight = 1000, const char* winTitle = "3D_Viewer") {return m_instance.init(winWidth, winHeight, winTitle);}
		static WindowMetrics GetWindowSize() { return m_instance.m_windowMetrics; }
		static GLFWwindow* GetGLFW_Window() { return m_instance.m_appWindow; }
		~Application();
	private:
		void InitImGui();
		bool init(int winWidth, int winHeight , const char* winTitle = "3D_Viewer");
	private:
		WindowMetrics m_windowMetrics;
		const char* m_winTitle;
		GLFWwindow* m_appWindow;
		static Application m_instance;
	};
	inline Application Application::m_instance;
}

