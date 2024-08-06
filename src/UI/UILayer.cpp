#include "pch.h"
#include "Enums/FIleImageEnums.h"
#include "Helpers/DialogWrapper.h"
#include "Helpers/TextureHelpers.h"
#include "Profiling/AppProfiler.h"
#include "UILayer.h"

enum ImageResolutionEnum_
{
	ImageResolutionEnum_k_640_X_480 = 0,
	ImageResolutionEnum_k_1280_X_720 = 1,
	ImageResolutionEnum_k_2560_X_1440 = 2,
	ImageResolutionEnum_k_3840_X_2160 = 3,
	ImageResolutionEnum_kCustom = 4,
};
constexpr ImGuiWindowFlags_ kBaseApplicationImGUIWindowStyleFlags = ImGuiWindowFlags_NoMove;


OBJ_Viewer::UILayer::UILayer(Application& application_ref):
	m_applicationRef(application_ref),
	m_UI_inputFramebuffer(application_ref.GetSceneViewport().GetViewportSize(), FramebufferAttachmentsFlags_kColorAttachment)
{

	this->m_imgGuiDockSpaceFlags = ImGuiDockNodeFlags_None;
	this->m_imGuiWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration;


	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

	if (this->m_imgGuiDockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoBackground;
}

void OBJ_Viewer::UILayer::isAppWindowFocused(APP_FOCUS_REGIONS::AppWindowID windowID)
{
	m_currentlyFocusedWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
		windowID : m_currentlyFocusedWindow;
}

void OBJ_Viewer::UILayer::OnEvent(Event& e)
{
    if (e.GetEventCategory() == EventCategory_kAppEvent && e.GetEventType() == EventType_kViewportSizeChanged)
    {
        auto& sceneViewportEvent = dynamic_cast<SceneViewportResizeEvent&>(e);
        m_UI_inputFramebuffer.ResizeFramebuffer(sceneViewportEvent.GetViewportSize());
    }

}

void OBJ_Viewer::UILayer::RenderUI(APP_SETTINGS::SceneConfigurationSettings& scene_config_settings_ref,
    std::weak_ptr<Model> scene_model, std::weak_ptr<MaterialRegistry> material_registry,
    std::weak_ptr<Skybox> scene_skybox)
{
#pragma region ImGUI frame setup
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::Begin("DockSpace Demo", (bool*)true, m_imGuiWindowFlags);
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_imgGuiDockSpaceFlags);
	}
#pragma endregion

#pragma region Menu bar
	static bool isScreenshotSettingsWindowOpen = false;
    static bool is_fbx_loading_disabled = true;
    static bool is_application_controls_window_open = false;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open file"))
			{
				EventOnSceneLoad e(is_fbx_loading_disabled);
				m_applicationRef.SubmitEvent(e);
			}
			if (ImGui::MenuItem("Take screenshot"))
			{
				isScreenshotSettingsWindowOpen = true;
				//Enter a screenshot UI state.
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::SameLine();
			ImGui::Checkbox("Disable fbx file imports", &is_fbx_loading_disabled);
			ImGui::SetItemTooltip("Due to security vulnerability fbx files are disabled. Enable them on you own risk or if the model is from trusted source");
			ImGui::Separator();

			ImGui::EndMenu();
		}
        if (ImGui::BeginMenu("Info"))
        {
            if (ImGui::MenuItem("Application controls."))
            {
                is_application_controls_window_open = true;
            }

            ImGui::EndMenu();
        }
		ImGui::EndMenuBar();
	}
#pragma endregion

#pragma region Controls window
    
    if (is_application_controls_window_open) {
        if (ImGui::Begin(APP_FOCUS_REGIONS::kUI_ApplicationControlInfoWindowName, &is_application_controls_window_open,
            ImGuiWindowFlags_NoDocking))
        {
            isAppWindowFocused(APP_FOCUS_REGIONS::kUI_ApplicationControlInfoWindowName);

            ImGui::Text("Camera rotation :LMB");
            ImGui::SetItemTooltip("Hold left mouse button to rotate the camera around.");

            ImGui::Text("Camera shifting :RMB");
            ImGui::SetItemTooltip("Hold right mouse button to shift the camera around.");

            ImGui::Text("Camera shift reset :'Q' ");
            ImGui::SetItemTooltip("Press 'Q' to reset the camera shift/pivot point");

            ImGui::Text("Hide UI :'H'");
            ImGui::SetItemTooltip("Press 'H' to hide or unhide the UI.");       
        } ImGui::End();
    }
#pragma endregion

#pragma region Screenshot window
    if (isScreenshotSettingsWindowOpen) {
        if (ImGui::Begin(APP_FOCUS_REGIONS::kUI_WindowScreenshotSettings, &isScreenshotSettingsWindowOpen, ImGuiWindowFlags_NoDocking))
        {
            isAppWindowFocused(APP_FOCUS_REGIONS::kUI_WindowScreenshotSettings);
            static const std::unordered_map<OBJ_Viewer::ImageFileFormat_, const char*> kUI_ImageFormatLabelMap =
            {
                 {OBJ_Viewer::ImageFileFormat_::ImageFileFormat_kPNG, "PNG"},
                 {OBJ_Viewer::ImageFileFormat_::ImageFileFormat_kJPEG ,"JPEG"},
                 {OBJ_Viewer::ImageFileFormat_::ImageFileFormat_kBMP ,"BMP"},

            };
            static const std::unordered_map<ImageResolutionEnum_, const char*> kUI_ResolutionOptionLabelMap =
            {
                  {ImageResolutionEnum_::ImageResolutionEnum_k_640_X_480, "SD 640 x 480"},
                  {ImageResolutionEnum_::ImageResolutionEnum_k_1280_X_720, "HD 1280 x 720"},
                  {ImageResolutionEnum_::ImageResolutionEnum_k_2560_X_1440,"2K 2560 x 1440"},
                  {ImageResolutionEnum_::ImageResolutionEnum_k_3840_X_2160,"4K 3840 x 2160"},
                  {ImageResolutionEnum_::ImageResolutionEnum_kCustom,"Custom"},
            };

            constexpr auto GetResolutionFromEnum = [](ImageResolutionEnum_ val)
                {
                    switch (val)
                    {
                    case ImageResolutionEnum_k_640_X_480:
                        return Size2D{ 640,480 };
                        break;
                    case ImageResolutionEnum_k_1280_X_720:
                        return Size2D{ 1280,720 };
                        break;
                    case ImageResolutionEnum_k_2560_X_1440:
                        return Size2D{ 2560,1440 };
                        break;
                    case ImageResolutionEnum_k_3840_X_2160:
                        return Size2D{ 3840,2160 };
                        break;
                    case ImageResolutionEnum_kCustom:
                        return Size2D{ 0,0 };
                        break;
                    default:
                        return Size2D{ 0,0 };
                        break;
                    }
                };

            static bool output_image_render_only_object{};
            static bool render_output_image_with_transparency{};
            static ImageFileFormat_ currently_selected_image_file_format = ImageFileFormat_::ImageFileFormat_kPNG;
            static ImageResolutionEnum_ currently_selected_resolution = ImageResolutionEnum_::ImageResolutionEnum_k_2560_X_1440;
            Size2D imageSize;

            currently_selected_image_file_format =
                RenderComboBox("Format", kUI_ImageFormatLabelMap, currently_selected_image_file_format);

            currently_selected_resolution =
                RenderComboBox("Image resolution", kUI_ResolutionOptionLabelMap, currently_selected_resolution);

            imageSize = GetResolutionFromEnum(currently_selected_resolution);

            if (currently_selected_resolution == ImageResolutionEnum_::ImageResolutionEnum_kCustom)
            {
                static Size2D previous_custom_resolution{};
                ImGui::Text("Custom resolution");
                ImGui::SameLine();
                ImGui::InputInt2("## Texture Size", &previous_custom_resolution[0]);
                imageSize = previous_custom_resolution;
            }

            ImGui::Checkbox("Render object only", &output_image_render_only_object);
            ImGui::SetItemTooltip("Don't render the grid and/or skybox.");

            ImGui::Checkbox("Export with transparency", &render_output_image_with_transparency);
            ImGui::SetItemTooltip("If the file format support it will export the scene with transparency.");

            if (ImGui::Button("Take screenshot"))
            {
                ScreenshotEvent e(ImgOutputData{ imageSize ,currently_selected_image_file_format,
                    output_image_render_only_object,render_output_image_with_transparency });
                m_applicationRef.SubmitEvent(e);
            }         
        } ImGui::End();
    }
	
#pragma endregion

#pragma region ModelAndRenderingSettings
	//Right panel for model and rendering settings.
	if (ImGui::Begin(APP_FOCUS_REGIONS::kUI_Model_and_renderingSettingsWindowName, (bool*)0, kBaseApplicationImGUIWindowStyleFlags))
	{
		isAppWindowFocused(APP_FOCUS_REGIONS::kUI_Model_and_renderingSettingsWindowName);

        if (std::shared_ptr<Model> current_scene_model = scene_model.lock())
        {
            static glm::vec3 position{};
            static glm::vec3 scale{};
            static glm::vec3 rotation{};
            static glm::vec3 previous_scale{};
            static bool is_scale_input_uniform = true;
            current_scene_model->GetMatrixDecomposed(position, rotation, scale);

            previous_scale = scale;

            ImGui::Text("Model view settings");
            ImGui::InputFloat3("Position", &position[0]);
            ImGui::InputFloat3("Rotation", &rotation[0]);
            ImGui::InputFloat3("Scale", &scale[0]);
            ImGui::Checkbox("Use uniform scale", &is_scale_input_uniform);

            if (is_scale_input_uniform)
            {
                float scaleValue = scale.x != previous_scale.x ? scale.x :
                    scale.y != previous_scale.y ? scale.y : scale.z;
                scale = glm::vec3(scaleValue);
            }
        }

		ImGui::Separator();
		if (ImGui::BeginChild("Rendering modes"))
		{
#pragma region Rendering Modes 

			if (ImGui::RadioButton("Wireframe",
				scene_config_settings_ref.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe))
			{
				scene_config_settings_ref.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe;
			}
			ImGui::SetItemTooltip("Renders wireframe of the 3D model.");

			if (ImGui::RadioButton("Normal orientation",
				scene_config_settings_ref.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kNormalOrientation))
			{
				scene_config_settings_ref.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kNormalOrientation;
			}
			ImGui::SetItemTooltip("Color each face based on the orientation of the normal red if its inwards and blue if outwards.");

			if (ImGui::RadioButton("Clear color rendering",
				scene_config_settings_ref.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor))
			{
				scene_config_settings_ref.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor;
			}
			ImGui::SetItemTooltip("Renders the mesh using a single color.");

			if (ImGui::RadioButton("Individual textures",
				scene_config_settings_ref.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture))
			{
				scene_config_settings_ref.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture;
			}
			ImGui::SetItemTooltip("Renders only single selected texture.");

			if (ImGui::RadioButton("Uv mode",
				scene_config_settings_ref.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kUV))
			{
				scene_config_settings_ref.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kUV;
			}
			ImGui::SetItemTooltip("Renders checkerboard texture for UV inspection.");

			if (ImGui::RadioButton("Light rendering",
				scene_config_settings_ref.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kLight))
			{
				scene_config_settings_ref.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kLight;
			}
			ImGui::SetItemTooltip("Render the 3D object with light calculations.");
#pragma endregion

			ImGui::Separator();
			ImGui::Text("Mode settings.");

			switch (scene_config_settings_ref.m_currentRenderingMode)
			{
			case APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe:
				ImGui::InputFloat("Wire line Thickness", &scene_config_settings_ref.wireframeSettings.lineThickness);
				ImGui::ColorPicker3("Line color", &(scene_config_settings_ref.wireframeSettings.lineColor)[0]);
				ImGui::Checkbox("Render points", &scene_config_settings_ref.wireframeSettings.isPointRenderingOn);
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kLight:
				static bool isAlbedoOn = scene_config_settings_ref.m_MaterialFlags & MaterialRenderingFalgs_kAlbedoIsOn;
				static bool isNormalOn = scene_config_settings_ref.m_MaterialFlags & MaterialRenderingFalgs_kSpecularIsOn;
				static bool isRoughnessOn = scene_config_settings_ref.m_MaterialFlags & MaterialRenderingFalgs_kNormalMapIsOn;
				static bool isAmbientOcclusionOn = scene_config_settings_ref.m_MaterialFlags & MaterialRenderingFalgs_kAmbientOcclusionIsOn;

				ImGui::Checkbox("Albedo?", &isAlbedoOn);
				ImGui::SetItemTooltip("Should the model display with the albedo/color texture.");

				ImGui::Checkbox("Specular?", &isRoughnessOn);
				ImGui::SetItemTooltip("Should the model display with the reflective/specular texture.");

				ImGui::Checkbox("Normals?", &isNormalOn);
				ImGui::SetItemTooltip("Should the model display with the normal map(apply on light calculations).");

				ImGui::Checkbox("Ambient occlusion?", &isAmbientOcclusionOn);
				ImGui::SetItemTooltip("Should the model display with the ambient occlusion texture.");

				scene_config_settings_ref.m_MaterialFlags = MaterialRenderingFalgs_kNone;
				scene_config_settings_ref.m_MaterialFlags = isAlbedoOn ? static_cast<MaterialRenderingFalgs_>(scene_config_settings_ref.m_MaterialFlags | MaterialRenderingFalgs_kAlbedoIsOn) : scene_config_settings_ref.m_MaterialFlags;
				scene_config_settings_ref.m_MaterialFlags = isNormalOn ? static_cast<MaterialRenderingFalgs_>(scene_config_settings_ref.m_MaterialFlags | MaterialRenderingFalgs_kNormalMapIsOn) : scene_config_settings_ref.m_MaterialFlags;
				scene_config_settings_ref.m_MaterialFlags = isRoughnessOn ? static_cast<MaterialRenderingFalgs_>(scene_config_settings_ref.m_MaterialFlags | MaterialRenderingFalgs_kSpecularIsOn) : scene_config_settings_ref.m_MaterialFlags;
				scene_config_settings_ref.m_MaterialFlags = isAmbientOcclusionOn ? static_cast<MaterialRenderingFalgs_>(scene_config_settings_ref.m_MaterialFlags | MaterialRenderingFalgs_kAmbientOcclusionIsOn) : scene_config_settings_ref.m_MaterialFlags;
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture:
				static bool selectedTexture;

				if (ImGui::RadioButton("Color texture",
					scene_config_settings_ref.m_curentIndividualTexture == MaterialTextures_kAlbedo))
				{
					scene_config_settings_ref.m_curentIndividualTexture = MaterialTextures_kAlbedo;
				}
				if (ImGui::RadioButton("Normal texture",
					scene_config_settings_ref.m_curentIndividualTexture == MaterialTextures_kNormal))
				{
					scene_config_settings_ref.m_curentIndividualTexture = MaterialTextures_kNormal;
				}
				if (ImGui::RadioButton("Specular/Metallic texture",
					scene_config_settings_ref.m_curentIndividualTexture == MaterialTextures_kMetalic))
				{
					scene_config_settings_ref.m_curentIndividualTexture = MaterialTextures_kMetalic;
				}
				if (ImGui::RadioButton("Ambient occlusion",
					scene_config_settings_ref.m_curentIndividualTexture == MaterialTextures_kAmbientOcclusion))
				{
					scene_config_settings_ref.m_curentIndividualTexture = MaterialTextures_kAmbientOcclusion;
				}
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kUV:
				ImGui::SliderFloat("Uv scale", &scene_config_settings_ref.m_uvViewSettings.UV_scaleFactor,
                    APP_SETTINGS::UV_ViewAppSetting::kUV_MinScaling, APP_SETTINGS::UV_ViewAppSetting::kUV_MaxScaling);
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor:
				ImGui::ColorPicker3("Mesh color", &scene_config_settings_ref.m_colorRenderingColor[0]);
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kNormalOrientation:

				break;
			default:
				break;
			}
		}ImGui::EndChild();
	}ImGui::End();
#pragma endregion

#pragma region Scene settings


	if (ImGui::Begin(APP_FOCUS_REGIONS::kUI_SceneSettingsWindowName))
	{
		isAppWindowFocused(APP_FOCUS_REGIONS::kUI_SceneSettingsWindowName);
		
		if (ImGui::BeginTabBar("Tab Scene materials"))
		{

			if (ImGui::BeginTabItem("Scene settings"))
			{
				ImGui::Text("Scene settings.");
				ImGui::Separator();

#pragma region Grid settings
				if (ImGui::CollapsingHeader("World grid settings."))
				{
					ImGui::Checkbox("UseWorldGrid?", &scene_config_settings_ref.m_isWireGridOn);
					if (scene_config_settings_ref.m_isWireGridOn)
					{
						ImGui::SliderFloat("Grid scale", &scene_config_settings_ref.m_gridData.gridScale,
                            APP_SETTINGS::GridData::kGridMinScale,
                            APP_SETTINGS::GridData::kGridMaxScale);

						ImGui::ColorPicker4("Grid color", &scene_config_settings_ref.m_gridData.gridLineColor[0]);
						ImGui::Checkbox("Shade axis.", &scene_config_settings_ref.m_gridData.isAxisShaded);
					}
				}
#pragma endregion

				ImGui::Separator();
#pragma region Light settings
				if (ImGui::CollapsingHeader("Light settings."))
				{
                    //If user go beyond 'MAX_LIGHT_COUNT' we use this formula to restrict it.
                    auto adjustLightCount = [](int& light_count_ref) {
                    /*Basically we have 4(as an example) as out max if we overshoot and go to 5 the expresion "MAX_LIGHT_COUNT - pSettings->lightInfo.lightCount"
                        will return negative value, a value that we can use to get the closest valid number in cases of 7 we get 7 += 4 - 7 <=> 7+=-3
                        and we use the min function in cases that we are within our range.We do it this way to avoid branching.
                        UPDATE: Added this for values below 0 as well by checking the value of 'lightCount' if its below zero we gonna add the positive version of it.
                        */
                        light_count_ref =
                            light_count_ref < 0 ? (light_count_ref - light_count_ref) :
                            light_count_ref + std::min(static_cast<size_t>(0), APP_SETTINGS::SceneLightInfo::kMaxLightCount - light_count_ref);
                        };

					static const std::unordered_map<APP_SETTINGS::LightShadingModel_,const char*> kShadingModeKeyValueComboBoxHash =
                    { {APP_SETTINGS::LightShadingModel_kBlinPhong,"Bling-Phong light shading"},
                      {APP_SETTINGS::LightShadingModel_kToonShading,"Toon light shading"},
                      {APP_SETTINGS::LightShadingModel_kRimShading,"Rim light shading"},
                      {APP_SETTINGS::LightShadingModel_kRim_and_ToonShading,"Rim + toon shading"} };

                    APP_SETTINGS::LightShadingModel_ current_light_model = scene_config_settings_ref.lightInfo.currentLightModel;
                    
                    scene_config_settings_ref.lightInfo.currentLightModel = 
                        RenderComboBox("Shading mode", kShadingModeKeyValueComboBoxHash, current_light_model);

					ImGui::InputInt("Light count", &scene_config_settings_ref.lightInfo.lightCount);

                    adjustLightCount(scene_config_settings_ref.lightInfo.lightCount);

					for (uint32_t i = 0; i < APP_SETTINGS::SceneLightInfo::kMaxLightCount; i++)
					{
						if (i < scene_config_settings_ref.lightInfo.lightCount)
						{
							if (ImGui::CollapsingHeader(std::string("Light " + std::to_string(i + 1)).c_str()))
							{
								RenderLightSettingsPanel(i,
									&scene_config_settings_ref.lightInfo.lights[i].color,
									&scene_config_settings_ref.lightInfo.lights[i].direction);
							}
						}
						else
						{
							scene_config_settings_ref.lightInfo.lights[i].color = glm::vec3(0);
						}
					}
				}
#pragma endregion
				ImGui::Separator();
#pragma region Skybox settings
				if (ImGui::CollapsingHeader("Skybox settings."))
				{
                    ImGui::Separator();

					ImGui::Checkbox("Enable skybox?", &scene_config_settings_ref.m_isSkyboxOn);

					if (scene_config_settings_ref.m_isSkyboxOn)
					{
                        if (ImGui::Button("Load skybox textures."))
                        {
                            EventOnSkyboxLoaded e;
                            m_applicationRef.SubmitEvent(e);
                        }
						RenderSkyboxSettings(scene_skybox);
					}
				}
#pragma endregion
				ImGui::Separator();

#pragma region Camera projection and AA settings

                static bool is_current_projection_perspective = true;
				if (ImGui::Button("Switch camera projection"))
				{
                    is_current_projection_perspective = !is_current_projection_perspective;
					EventCameraProjectionChanged e(is_current_projection_perspective);
					m_applicationRef.SubmitEvent(e);
				}

				ImGui::SetItemTooltip("Current projection mode is:%s", is_current_projection_perspective ? "Perspective" : "Orthographic");
				ImGui::Checkbox("Enable AA", &scene_config_settings_ref.m_EnableAA);
				ImGui::SetItemTooltip("Enable scene Anti-aliasing(AA).The scene AA is using MSAA + FXAA");
#pragma endregion

				ImGui::EndTabItem();
			}

#pragma region Scene mesh info
			if (ImGui::BeginTabItem("Scene/Model info"))
			{
                if (std::shared_ptr<Model> current_scene_model = scene_model.lock())
                {
                    const ModelData& current_model_load_data = current_scene_model->GetModelData();
                    ImGui::Text("Model info");
                    ImGui::Text("Object triangle count:%d", current_model_load_data.meshInfo.triangleCount);
                    ImGui::Text("Object vertex count:%d", current_model_load_data.meshInfo.vertexCount);
                    ImGui::Text("Object face count:%d", current_model_load_data.meshInfo.faceCount);
                    ImGui::Text("Texture count:%d", current_model_load_data.textureCount);

                    ImGui::Text("File path %s", current_model_load_data.modelPath.c_str());
                    ImGui::SetItemTooltip("Path:%s", current_model_load_data.modelPath.c_str());
                }

				ImGui::Separator();
#pragma endregion

#pragma region Render scene materials
				if (std::shared_ptr<MaterialRegistry> MaterialRegistry = material_registry.lock())
				{
					ImGui::Text("Scene materials");

					for (size_t i = 0; i < MaterialRegistry->GetMaterialCount(); i++)
					{
                        const auto& material = MaterialRegistry->GetMaterialAtIndex(i).lock();

						if (material && ImGui::CollapsingHeader(material->GetMaterialName().c_str()))
						{
							//Add imported textures preview
							RenderMaterial_LabelTexturePair(material, MaterialTextures_::MaterialTextures_kAlbedo, "Albedo");

							RenderMaterial_LabelTexturePair(material, MaterialTextures_::MaterialTextures_kNormal, "Normal");

							RenderMaterial_LabelTexturePair(material, MaterialTextures_::MaterialTextures_kMetalic, "Specular");

							RenderMaterial_LabelTexturePair(material, MaterialTextures_::MaterialTextures_kAmbientOcclusion,
								"Ambient occlusion");
						}				
					}
					ImGui::Separator();
				}
#pragma endregion

#pragma region  Render scene hierarchy
				//ImGui::Text("Scene hierarchy");
				//constexpr size_t SCENE_OBJECT_COUNT_TEST = 5;
				//for (size_t i = 0; i < SCENE_OBJECT_COUNT_TEST; i++)
				//{
				//	ImGui::Text("Name of mesh");
				//	ImGui::SameLine();
				//	ImGui::PushItemWidth(-1);
				//	if (ImGui::RadioButton(std::string("##" + std::to_string(i + 1)).c_str(), true))
				//	{
				//		LOGGER_LOG_INFO("OBJECT_VISIBILITY IS CHANGED");
				//	}
				//	ImGui::PopItemWidth();
				//}
				ImGui::EndTabItem();
			}

		}ImGui::EndTabBar();
#pragma endregion
	}ImGui::End();

#pragma endregion

#pragma region Loading object info
	/*if (ImGui::Begin(APP_FOCUS_REGIONS::UI_LAYER_OBJECT_LOADING_WINDOW_NAME))
	{
		isAppWindowFocused(APP_FOCUS_REGIONS::UI_LAYER_OBJECT_LOADING_WINDOW_NAME);
	}ImGui::End();*/
#pragma endregion

#pragma region Scene window

	if(ImGui::Begin(APP_FOCUS_REGIONS::kUI_SceneWindowName,(bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse))
	{
		isAppWindowFocused(APP_FOCUS_REGIONS::kUI_SceneWindowName);

		ImVec2 winSize = ImGui::GetWindowSize();
		static Viewport sceneWinViewport{};
		sceneWinViewport.width = winSize.x;
		sceneWinViewport.height = winSize.y;

		m_UI_inputFramebuffer.ResizeFramebuffer(Size2D{ sceneWinViewport.width,sceneWinViewport.height });

        if(!m_isFirstFrame)
		    m_applicationRef.SubmitSceneViewportSize(sceneWinViewport);

		ImGui::BeginChild("GameRender", winSize,ImGuiChildFlags_None, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::Image((ImTextureID)m_UI_inputFramebuffer.GetFramebufferTexture().GetTextureHandle(), winSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::EndChild();

	}ImGui::End();
#pragma endregion
  
#pragma region Submit UI

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

	m_applicationRef.GetGlobalInputHandler().SetCurrentlyFocusedWindow(m_currentlyFocusedWindow);
    m_isFirstFrame = false;
    //pScene_model->ApplyTransformation(position, scale, glm::vec3(1), 0);
}

void OBJ_Viewer::UILayer::RenderMaterial_LabelTexturePair(const std::shared_ptr<OBJ_Viewer::Material>& material,
	MaterialTextures_ textureType,const char* textureLabelName)
{
#pragma region Style settings
	static constexpr float spacing = 100.f;
	constexpr uint8_t RIGHT_SIDE_IMAGE_MARGIN = 10;
	static constexpr GLuint TEXTURE_HANDLE_ID_NONE = 0;
	static constexpr ImVec2 textSize = { 70,70 };
	const ImVec2 currentWindowSize = ImGui::GetWindowSize();
	const float IMAGE_TEXT_INDEPENDENT_POSITION = currentWindowSize.x - textSize.x - RIGHT_SIDE_IMAGE_MARGIN;
#pragma endregion

	ImGui::Text(textureLabelName);
	ImGui::SameLine(IMAGE_TEXT_INDEPENDENT_POSITION);
	GLuint textureID =
		material->GetMaterialTexture(textureType).expired() ? TEXTURE_HANDLE_ID_NONE :
		material->GetMaterialTexture(textureType).lock()->GetTextureHandle();

	ImGui::Image((ImTextureID)textureID, textSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
}

void OBJ_Viewer::UILayer::RenderSkyboxSettings(std::weak_ptr<Skybox> scene_skybox)
{
    const static std::unordered_map<OBJ_Viewer::SkyboxFace_, const char*> kUI_SkyboxFaceLabelMap =
    {
        {OBJ_Viewer::SkyboxFace_kRight, "Right face"},
        {OBJ_Viewer::SkyboxFace_kLeft ,"Left face"},
        {OBJ_Viewer::SkyboxFace_kTop,"Top face"},
        {OBJ_Viewer::SkyboxFace_kBottom,"Bottom face"},
        {OBJ_Viewer::SkyboxFace_kFront,"Front face"},
        {OBJ_Viewer::SkyboxFace_kBack,"Back face"}
    };

	if (std::shared_ptr<Skybox> scene_sykbox = scene_skybox.lock())
	{
		const std::array<std::shared_ptr<Texture>,Skybox::SKYBOX_FACE_COUNT>& skyboxTextures = scene_sykbox->GetSkyboxFaceTextures();

		static SkyboxFace_ comboResult{};
		static std::string index_to_string = "## ";
		for (uint8_t i = 0; i < Skybox::SKYBOX_FACE_COUNT; i++)
		{
			ImGui::Image((ImTextureID)skyboxTextures[i]->GetTextureHandle(),{ 50,50 }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
			ImGui::SameLine();
			index_to_string[index_to_string.size() - 1] = '0' + i;
			comboResult = RenderComboBox<SkyboxFace_>(index_to_string, kUI_SkyboxFaceLabelMap,
				static_cast<SkyboxFace_>(i));

			if(i != comboResult)
                scene_sykbox->SwapSkyboxFaceTextures(static_cast<SkyboxFace_>(i), comboResult);
		}

		ImGui::Separator();
	}
}

void OBJ_Viewer::UILayer::RenderLightSettingsPanel(uint32_t lightIndex,glm::vec3 *pColor, glm::vec3* pPosition)
{
	ImGui::ColorPicker3(std::string("Light color:" + std::to_string(lightIndex)).c_str(),&(*pColor)[0]);
	ImGui::NewLine();
	ImGui::VSliderFloat(std::string("Light direction.x:" + std::to_string(lightIndex)).c_str(), { 50,50}, &(pPosition->x), -1.f, 1.f);
	ImGui::VSliderFloat(std::string("Light direction.y:" + std::to_string(lightIndex)).c_str(), { 50,50 }, &(pPosition->y), -1.f, 1.f);
	ImGui::VSliderFloat(std::string("Light direction.z:" + std::to_string(lightIndex)).c_str(), { 50,50 }, &(pPosition->z), -1.f, 1.f);

}
