#pragma once
#include "pch.h"
#include "Core/Application.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderingMediator.h"

namespace OBJ_Viewer {

	class UILayer
	{
	public:
		UILayer(Application& appState, std::shared_ptr<RenderingMediator> renderingMediator);
		void RenderUI();
		Framebuffer& GetInputFramebuffer() { return m_UI_inputFramebuffer; }
	private:
		template<typename T>
		T RenderComboBox(const std::string& comboLabel, const std::unordered_map<T, const char*>& items, const T& previewItem);
		void RenderSkyboxSettings();
		void RenderLightSettingsPanel(uint32_t lightIndex, glm::vec3* pColor, glm::vec3* pPosition);
		void RenderMaterial_LabelTexturePair(const std::shared_ptr<OBJ_Viewer::Material>& material,
			MaterialTextures textureType, const char* textureLabelName);
		void isAppWindowFocused(APP_FOCUS_REGIONS::AppWindowID windowID);
	private:
		APP_FOCUS_REGIONS::AppWindowID m_currentlyFocusedWindow;
		Application& m_application;
		Framebuffer m_UI_inputFramebuffer;
		std::shared_ptr<RenderingMediator> m_mediator;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;
	};
	template<typename T>
	inline T UILayer::RenderComboBox(const std::string& comboLabel, const std::unordered_map<T, const char*>& items,
		const T& previewItem)
	{
		if (ImGui::BeginCombo(comboLabel.c_str(), items.at(previewItem))) // The second parameter is the label previewed before opening the combo.
		{
			for (const auto [key, value] : items)
			{
				bool is_selected = (items.at(previewItem) == value); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(value, is_selected))
				{
					ImGui::EndCombo();
					return key;
				}
			}
			ImGui::EndCombo();
		}

		return previewItem;
	}
}

