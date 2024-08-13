#pragma once
#include "pch.h"
#include "Core/Application.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/SceneConfigurationSettingsStruct.h"
namespace OBJ_Viewer {

    /**
     * @brief Class that is responsible for rendering the UI.
     *
     * This class works with ImGUI to render the UI.
     * The entire UI is contained in this class.
     * The caller must call 'RenderUI(<params>)' to render the UI.
     * When we render the UI we also retrieve the information about the UI(witch checkbox has been checked etc)
     * all of this information is contained in a 'APP_SETTINGS::SceneConfigurationSettings' struct.
     * The class also contains its own UI framebuffer.
     * 
     */
	class UILayer : public Listener
	{
	public:
        /**
         * @brief Sets the ImGUI for dock space rendering. 
         * 
         * Constructor creates the UI framebuffer and sets the ImGUI for dock space rendering.
         * It also copies the main application object reference(this is use for event submission)
         * 
         * @param applicationRef The main application reference.
         */
        UILayer(Application& applicationRef);
        /**
         * @brief Renders the UI and retrieves user-submitted information.
         *
         * Since ImGUI operates in immediate mode, the UI must be rendered every frame. This function encapsulates all the UI rendering code.
         *
         * The function accepts a reference to 'APP_SETTINGS::SceneConfigurationSettings', which is used to collect user input.
         * A reference is passed instead of returning a copy to avoid the overhead of copying, especially given the 60 frames per second rendering rate.
         *
         * The UI can also trigger events using the 'm_applicationRef' member, such as when a button is pressed. This allows the UI to delegate event handling
         * to other classes using an event-driven approach, leading to more maintainable code. The UI interacts with both scene data (Model, MaterialRegistry, and Skybox).
         *
         * - MaterialRegistry: Provides the user with information about the loaded materials.
         * - Model: Currently, only one piece of information (ModelUIData) is used by the UI.
         * - Skybox: Facilitates reading and swapping skybox faces (which could potentially evolve into its own event in the future).
         *
         * @param scene_config_settings_ref Reference to the scene configuration settings, used to store user input.
         * @param scene_model Weak pointer to the scene's model, used to retrieve model-specific data for the UI.
         * @param material_registry Weak pointer to the material registry, used to display information about loaded materials.
         * @param scene_skybox Weak pointer to the skybox, used to manage skybox settings, such as swapping faces.
         * 
         * 
         * @note It may seem unusual to use `std::weak_ptr` instead of references in this context, but there is a valid reason for this choice.
         *       The UI can trigger events that may modify or update the scene, including changes to the `Model`, `Skybox`, or `MaterialRegistry`.
         *       Since the application is primarily single-threaded (as OpenGL does not support multi-threading), using `std::weak_ptr` helps to avoid issues related to object lifetimes and potential conflicts.
         *       When the UI triggers an event, it submits the event to the application, which then notifies all relevant components. For example, if a new model is loaded, it will replace the previous model.
         *       Using `std::weak_ptr` helps prevent potential problems such as dangling references or accessing invalid objects, ensuring safer and more manageable code in such dynamic scenarios.
         */
        void RenderUI(APP_SETTINGS::SceneConfigurationSettings& scene_config_settings_ref,std::weak_ptr<Model> scene_model,
            std::weak_ptr<MaterialRegistry> material_registry,
            std::weak_ptr<Skybox> scene_skybox);
        /**
         * @brief Returns the Scene UI input framebuffer.
         *
         * @note The UI framebuffer might be moved in the RenderingCoordinator in the future.
         * @returns Returns the Scene UI input framebuffer.
         */
		Framebuffer& GetInputFramebuffer() { return m_UI_inputFramebuffer; }
	private:
        /**
         * @brief Utility function for rendering a combo box element.
         *
         * This templated function renders a combo box with a set of items and returns the selected item.
         * It requires a hash map where the keys are the values to be retrieved by the combo box,
         * and the values are the corresponding labels displayed in the UI.
         *
         * Example use:
         * ```cpp
         * //You can define these at the top of the UILayer source file or within the render UI function.
         * 
         * static const std::unordered_map<ImageResolutionEnum_, const char*> kUI_ResolutionOptionLabelMap =
         * {
         *       {ImageResolutionEnum_::ImageResolutionEnum_k_640_X_480, "SD 640 x 480"},
         *       {ImageResolutionEnum_::ImageResolutionEnum_k_1280_X_720, "HD 1280 x 720"},
         *       {ImageResolutionEnum_::ImageResolutionEnum_k_2560_X_1440,"2K 2560 x 1440"},
         *       {ImageResolutionEnum_::ImageResolutionEnum_k_3840_X_2160,"4K 3840 x 2160"},
         *       {ImageResolutionEnum_::ImageResolutionEnum_kCustom,"Custom"},
         * };
         * 
         * currently_selected_resolution =
         *     RenderComboBox("Image resolution", kUI_ResolutionOptionLabelMap, currently_selected_resolution);
         * ```
         * 
         * @tparam T The type of the items in the combo box.
         *
         * @param comboLabel The label for the combo box.
         * @param items A hash map where each key-value pair represents an item in the combo box:
         *              - Key: The value that will be retrieved when the item is selected.
         *              - Value: The label displayed for the item in the combo box.
         * @param previewItem The currently selected item or the default item to be displayed in the combo box.
         *
         * @return T The key of the selected item from the combo box(if none selected the returned item will be the 
         * currently selected one).
         */
		template<typename T>
		T RenderComboBox(const std::string& comboLabel, const std::unordered_map<T, const char*>& items, const T& previewItem);
        /**
         * @brief Renders skybox related settings.
         *
         * This function renders the skybox face swap combo box 
         * @param scene_skybox The scene skybox.
         */
		void RenderSkyboxSettings(std::weak_ptr<Skybox> scene_skybox);
        /**
         * @brief Renders an individual light color picker and position sliders.
         * 
         * This function renders a color picker for the light color and 3
         * light direction sliders that are from [-1:1].
         * 
         * @param lightIndex Create a label "Light color:<index of the light>" 
         * @param pColor Output for UI light color picker.
         * @param pLightDirection Output for the light direction.
         */
		void RenderLightSettingsPanel(uint32_t lightIndex, glm::vec3* pColor, glm::vec3* pLightDirection);
        /**
         * @brief Renders a texture label and the texture image at the same line.
         *
         * This function displays a label and a material texture image on the same line. If the label is longer (such as "Ambient Occlusion"),
         * the function adjusts the layout so that the every texture in this pair will be one above or below another 
         * 
         * <label>--<Label independent offset>-------[Image]
         * <long label>--<Label independent offset>--[Long label Image]
         *
         * If the material don't have such texture the image will be a black image.
         * 
         * @param material A shared pointer to an `OBJ_Viewer::Material` object, which provides the texture to be rendered.
         * @param textureType Specifies the type of texture to be rendered from the material.
         * @param textureLabelName A string representing the label to be displayed alongside the texture image.
         */
		void RenderMaterial_LabelTexturePair(const std::shared_ptr<OBJ_Viewer::Material>& material,
			MaterialTextures_ textureType, const char* textureLabelName);
        /**
         * @brief Checks if the specified application window is currently focused and updates the focus state.
         *
         * This function uses ImGUI to determine if the specified window is currently focused by the mouse.
         * If the window is focused, it invokes the application input handler and updates the currently focused UI window.
         *
         * @param windowID The identifier of the application window to check for focus.
         */
		void isAppWindowFocused(APP_FOCUS_REGIONS::AppWindowID windowID);
	private:
		APP_FOCUS_REGIONS::AppWindowID m_currentlyFocusedWindow;
		Application& m_applicationRef;///< Main application reference.
		Framebuffer m_UI_inputFramebuffer; ///< The scene input framebuffer(This might be moved in the coordinator and be passed as a parameter.) 
		ImGuiWindowFlags m_imGuiWindowFlags;//< ImGUI window flags.
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;//< ImGUI docking flags.
        bool m_isFirstFrame = true; ///< Is the currently rendering frame the first(This is reserved might be moved as a static variable.)
        /**
         * @brief Currently used for the UI framebuffer resize. 
         */
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

