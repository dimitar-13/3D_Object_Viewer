#pragma once
#include <iostream>
#include<imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"ModelLoader.h"
#include"ShaderClass.h"
#include"Camera.h"
#include"WindowHandler.h"
#include <nfd.h>
#include"ShaderPath.h"
#include<memory>
namespace OBJ_Viewer
{
	class Renderer
	{
	public:
		void RenderObject(const ShaderClass& shaderToUse, const Model& modelToRender, const Camera& mainCamera);
		void EnableWireFrame() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		void DisableWireFrame(){ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
		//void DrawSkybox();
	};

	struct RendererSettings
	{
		bool m_isWireFrameRenderingOn;
		bool m_isRenderAlbedoTextureOn;
		bool m_isRenderSpecularTextureOn;
		bool m_isRenderNormalTextureOn;
		bool m_isSkyboxOn;
		bool m_isWireGridOn;
		bool m_isRenderAmbientOcclusionTextureOn;
		bool m_isRenderingLightOn;
	};
	struct RendererShaders {
		RendererShaders() :colorShader(GetConcatShaderPath("ColorShader.glsl").c_str())/*, lightShader("LightSHaderPath"), skyboxShader("SkyboxPath")*/ {}
		ShaderClass colorShader;
		//ShaderClass lightShader;
		//ShaderClass skyboxShader;
	};
	class RenderingCoordinator {
	public:
		RenderingCoordinator(Window* m_windowHandler);
		void RenderLoop();
		void RenderScene();
		void RenderImGui();
	private:
		RendererSettings m_rendererSettings;
		std::shared_ptr<Model> m_currentlyLoadedModel;
		std::unique_ptr<Camera> m_Camera;
		Window* m_windowHandler = nullptr;
		RendererShaders m_rendererShaders;
		Renderer m_mainRenderer;
	};
}

	

