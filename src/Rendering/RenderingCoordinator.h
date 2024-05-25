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
#include"UIRenderer.h"
namespace OBJ_Viewer
{
	struct RendererSettings
	{
		bool m_isWireFrameRenderingOn;
		bool m_isSkyboxOn;
		bool m_isWireGridOn;
		bool m_isRenderingLightOn;

		bool m_isRenderAlbedoTextureOn;
		bool m_isRenderSpecularTextureOn;
		bool m_isRenderNormalTextureOn;
		bool m_isRenderAmbientOcclusionTextureOn;
	};
	

	struct RendererShaders {
		RendererShaders() :colorShader(GetConcatShaderPath("ColorShader.glsl").c_str())/*, lightShader("LightSHaderPath"), skyboxShader("SkyboxPath")*/ {}
		ShaderClass colorShader;
		//ShaderClass lightShader;
		//ShaderClass skyboxShader;
	};
	class RenderingCoordinator{
	public:
		RenderingCoordinator(Window* m_windowHandler, InputHandler* pInputHandler);
		void RenderLoop();
	private:
		void RenderScene();
		void RenderImGui();
		void LoadModel();
	private:
		std::shared_ptr<Model> m_currentlyLoadedModel;
		std::unique_ptr<Camera> m_Camera;
		Window* m_windowHandler = nullptr;
		RendererSettings m_rendererSettings;
		RendererShaders m_rendererShaders;
		UIRenderer m_imGuiUIRenderer;
		Framebuffer m_sceneFramebuffer;
		InputHandler* m_pInputHandler = nullptr;
	};
}


	

