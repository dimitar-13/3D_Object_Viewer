#pragma once
#include <iostream>
#include<imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"ModelLoader.h"
#include <nfd.h>
namespace OBJ_Viewer
{
	static class RenderingSettings
	{
	public:
		//Might move to cpp file.
		static inline bool m_isWireFrameRenderingOn;
		static inline bool m_isRenderAlbedoTextureOn;
		static inline bool m_isRenderSpecularTextureOn;
		static inline bool m_isRenderNormalTextureOn;
		static inline bool m_isSkyboxOn;
		static inline bool m_isWireGridOn;
		static inline bool m_isRenderAmbientOcclusionTextureOn;
		static inline bool m_isRenderingLightOn;
		//TODO:change from void to a Model struct representing a model.
		static inline Model* currentlyLoadedModel = nullptr;
	public:
		static void RenderLoop(GLFWwindow* mainWin);
		static void RenderScene();
		static void RenderImGui();
		static nfdchar_t* OpenDialog();
	};
}

