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
#include <nfd.h>
namespace OBJ_Viewer
{
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
		RendererShaders() :colorShader("ColorShaderPath"), lightShader("LightSHaderPath"), skyboxShader("SkyboxPath") {}
		ShaderClass colorShader;
		ShaderClass lightShader;
		ShaderClass skyboxShader;
	};

	class Renderer
	{
	public:
		Renderer();
	public:
		RendererSettings m_rendererSettings;
		Model* currentlyLoadedModel = nullptr;
		RendererShaders m_rendererShaders;
		Camera m_Camera;
	public:
		 void RenderLoop();
		 void RenderScene();
		 void RenderImGui();
		 nfdchar_t* OpenDialog();
	};
}

