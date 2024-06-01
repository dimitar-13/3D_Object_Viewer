#pragma once
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"ModelLoader.h"
#include"ShaderClass.h"
#include"Scene/Camera.h"
#include"WindowHandler.h"
#include <nfd.h>
#include"ShaderPath.h"
#include<memory>
#include"Framebuffer.h"
#include"UI/UILayer.h"
#include "AppState.h"
namespace OBJ_Viewer
{
	struct RendererShaders {
		RendererShaders() :colorShader(GetConcatShaderPath("ColorShader.glsl").c_str()),
			skyboxShader(GetConcatShaderPath("SkyboxShader.glsl").c_str())/*, lightShader("LightSHaderPath"), skyboxShader("SkyboxPath")*/ {}
		ShaderClass colorShader;
		//ShaderClass lightShader;
		ShaderClass skyboxShader;
	};
	class RenderingCoordinator{
	public:
		RenderingCoordinator(AppState* appState);
		void RenderLoop();
	private:
		void RenderScene();
	private:
		std::unique_ptr<Camera> m_Camera;
		AppState* m_appState;
		UILayer m_UILayer;
		RendererShaders m_rendererShaders;
	};
}


	

