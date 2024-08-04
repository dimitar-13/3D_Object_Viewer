#pragma once
#include "pch.h"
#include "Core/Application.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/SceneConfigurationSettingsStruct.h"
namespace OBJ_Viewer {

	class UILayer : public Listener
	{
	public:
        UILayer(Application& appState);
        void RenderUI(APP_SETTINGS::SceneConfigurationSettings& scene_config_settings_ref,std::weak_ptr<Model> scene_model,
            std::weak_ptr<MaterialRegistry> material_registry,
            std::weak_ptr<Skybox> scene_skybox);
		Framebuffer& GetInputFramebuffer() { return m_UI_inputFramebuffer; }
	private:
		template<typename T>
		T RenderComboBox(const std::string& comboLabel, const std::unordered_map<T, const char*>& items, const T& previewItem);
		void RenderSkyboxSettings(std::weak_ptr<Skybox> scene_skybox);
		void RenderLightSettingsPanel(uint32_t lightIndex, glm::vec3* pColor, glm::vec3* pPosition);
		void RenderMaterial_LabelTexturePair(const std::shared_ptr<OBJ_Viewer::Material>& material,
			MaterialTextures_ textureType, const char* textureLabelName);
		void isAppWindowFocused(APP_FOCUS_REGIONS::AppWindowID windowID);
	private:
		APP_FOCUS_REGIONS::AppWindowID m_currentlyFocusedWindow;
		Application& m_applicationRef;
		Framebuffer m_UI_inputFramebuffer;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;

        // Inherited via Listener
        void OnEvent(Event& e) override;
    };
	template<typename T>
	inline T UILayer::RenderComboBox(const std::string& comboLabel, const std::unordered_map<T, const char*>& items,
		const T& previewItem)
	{
		if (ImGui::BeginCombo(comboLabel.c_str(), items.at(previewItem)))
		{
			for (const auto [key, value] : items)
			{
				bool is_selected = (items.at(previewItem) == value);
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

