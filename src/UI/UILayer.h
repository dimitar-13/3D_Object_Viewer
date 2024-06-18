#pragma once
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include"AppState.h"
#include<glm/glm.hpp>
#include"Rendering/SceneRenderer.h"
#include<memory>

namespace OBJ_Viewer {

constexpr auto UI_LAYER_SCENE_WINDOW_NAME = "Scene";
constexpr auto UI_LAYER_OBJECT_LOADING_WINDOW_NAME = "Loading panel";
constexpr auto UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME = "ModelAndRenderingSettings";
constexpr auto UI_LAYER_SCENE_SETTINGS_WINDOW_NAME = "Scene setting";
constexpr auto UI_WINDOW_UNKNOWN = "unknownWindow";
	class UILayer
	{
	public:
		UILayer(AppState* appState, std::shared_ptr<SceneRenderer> sceneRenderer,ImGuiWindowFlags imGuiWindowFlags,
			ImGuiDockNodeFlags imGuiDockSpaceFlags);
		void RenderUI();
	private:
		void LoadModel();
		void LoadSkybox();
		void RenderComboBox(std::string comboLabel, int index);
		void RenderSkyboxSettings();
		void RenderLightSettingsPanel(uint32_t lightIndex, glm::vec3* pColor, glm::vec3* pPosition);
	private:
	
		AppState* m_appState;
		std::shared_ptr<SceneRenderer> m_sceneRenderer;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;
	};
}

