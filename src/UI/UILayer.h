#pragma once
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"ModelLoader.h"
#include"Rendering/ShaderClass.h"
#include"Scene/Camera.h"
#include"WindowHandler.h"
#include <nfd.h>
#include"ShaderPath.h"
#include<memory>
#include"AppState.h"
namespace OBJ_Viewer {

constexpr auto UI_LAYER_SCENE_WINDOW_NAME = "Scene";
constexpr auto UI_LAYER_OBJECT_LOADING_WINDOW_NAME = "Loading panel";
constexpr auto UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME = "ModelAndRenderingSettings";
constexpr auto UI_LAYER_SCENE_SETTINGS_WINDOW_NAME = "Scene setting";
constexpr auto UI_WINDOW_UNKNOWN = "unknownWindow";
	class UILayer
	{
	public:
		UILayer(AppState* appState,ImGuiWindowFlags imGuiWindowFlags,
			ImGuiDockNodeFlags imGuiDockSpaceFlags);
		void RenderUI();
	private:
		void LoadModel();
		void LoadSkybox();
		void RenderComboBox(std::string comboLabel, int index);
		void RenderSkyboxSettings(const Skybox* skybox);
		void RenderLightSettingsPanel(uint32_t lightIndex, glm::vec3* pColor, glm::vec3* pPosition);
	private:
	
		AppState* m_appState;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;
	};
}

