#pragma once
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include"Application.h"
#include<glm/glm.hpp>
#include"Rendering/SceneRenderer.h"
#include"RenderingMediator.h"
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
		UILayer(Application& appState, std::shared_ptr<RenderingMediator> renderingMediator,ImGuiWindowFlags imGuiWindowFlags,
			ImGuiDockNodeFlags imGuiDockSpaceFlags);
		void RenderUI();
	private:
		void LoadModel();
		void LoadSkybox();
		void RenderComboBox(std::string comboLabel, int index);
		void RenderSkyboxSettings();
		void RenderLightSettingsPanel(uint32_t lightIndex, glm::vec3* pColor, glm::vec3* pPosition);
	private:
	
		Application& m_application;
		std::shared_ptr<RenderingMediator> m_mediator;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;
		std::function<void(Event&)> m_appEventCallback;
	};
}

