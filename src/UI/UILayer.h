#pragma once
#include "pch.h"
#include "Core/Application.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderingMediator.h"

namespace OBJ_Viewer {

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
		void RenderMaterial_LabelTexturePair(const std::shared_ptr<OBJ_Viewer::Material>& material,
			MaterialTextures textureType, const char* textureLabelName);
		void isAppWindowFocused(APP_FOCUS_REGIONS::AppWindowID windowID);
	private:
		APP_FOCUS_REGIONS::AppWindowID m_currentlyFocusedWindow;
		Application& m_application;
		std::shared_ptr<RenderingMediator> m_mediator;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;
		std::function<void(Event&)> m_appEventCallback;
	};
}

