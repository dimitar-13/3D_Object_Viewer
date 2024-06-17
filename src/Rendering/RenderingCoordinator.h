#pragma once
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"WindowHandler.h"
#include <nfd.h>
#include"ShaderPath.h"
#include<memory>
#include"Framebuffer.h"
#include"UI/UILayer.h"
#include "AppState.h"
#include "SceneRenderer.h" 
namespace OBJ_Viewer
{
	class RenderingCoordinator{
	public:
		RenderingCoordinator(AppState* appState);
		void RenderLoop();
	private:
		void RenderScene();
	private:
		AppState* m_appState;
		std::unique_ptr<UILayer> m_UILayer;
		std::shared_ptr<SceneRenderer> m_sceneRenderer;
	};
}


	

