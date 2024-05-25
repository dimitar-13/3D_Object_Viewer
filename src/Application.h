#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"Rendering/Renderer.h"
#include"WindowHandler.h"
namespace OBJ_Viewer {

	class Application
	{
	public:
		Application();
		Window& GetAppWindow() { return *this->m_window; }
		~Application();
	private:
		void InitImGui();
	private:
		Window* m_window;
	};
}

