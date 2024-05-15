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
namespace OBJ_Viewer
{
	class Renderer
	{
	public:
		void RenderObject(const ShaderClass& shaderToUse, const Model& modelToRender, const Camera& mainCamera);
	};

	struct RendererSettings
	{
		static inline bool m_isWireFrameRenderingOn;
		static inline bool m_isRenderAlbedoTextureOn;
		static inline bool m_isRenderSpecularTextureOn;
		static inline bool m_isRenderNormalTextureOn;
		static inline bool m_isSkyboxOn;
		static inline bool m_isWireGridOn;
		static inline bool m_isRenderAmbientOcclusionTextureOn;
		static inline bool m_isRenderingLightOn;
	};
	struct RendererShaders {
		RendererShaders() :colorShader(GetConcatShaderPath("ColorShader.glsl").c_str())/*, lightShader("LightSHaderPath"), skyboxShader("SkyboxPath")*/ {}
		ShaderClass colorShader;
		//ShaderClass lightShader;
		//ShaderClass skyboxShader;
	};
	class RendererCoordinator {
	public:
		RendererCoordinator(Window* m_windowHandler);
		void RenderLoop();
		void RenderScene();
		void RenderImGui();
		nfdchar_t* OpenDialog();
	private:
		RendererSettings m_rendererSettings;
		Model* currentlyLoadedModel = nullptr;
		Camera* m_Camera;
		Window* m_windowHandler = nullptr;
		RendererShaders m_rendererShaders;
		Renderer m_mainRenderer;
	};
}

	

