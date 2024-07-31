#include "pch.h"
#include "UILayer.h"
#include "Helpers/DialogWrapper.h"
#include "Profiling/AppProfiler.h"
#include "Enums/FIleImageEnums.h"
#include "Helpers/TextureHelpers.h"

enum ImageResolutionEnum_
{
	ImageResolutionEnum_k_640_X_480 = 0,
	ImageResolutionEnum_k_1280_X_720 = 1,
	ImageResolutionEnum_k_2560_X_1440 = 2,
	ImageResolutionEnum_k_3840_X_2160 = 3,
	ImageResolutionEnum_kCustom = 4,
};
constexpr ImGuiWindowFlags_ APP_WINDOW_STYLE_FLAGS = ImGuiWindowFlags_NoMove;

#pragma region KEY_UI_LABEL_MAPS
inline const std::unordered_map<OBJ_Viewer::SkyboxFace, const char*> kUI_SkyboxFaceLabelMap =
{
	{OBJ_Viewer::SKYBOX_FACE_RIGHT, "Right face"},
	{OBJ_Viewer::SKYBOX_FACE_LEFT ,"Left face"},
	{OBJ_Viewer::SKYBOX_FACE_TOP,"Top face"},
	{OBJ_Viewer::SKYBOX_FACE_BOTTOM,"Bottom face"},
	{OBJ_Viewer::SKYBOX_FACE_FRONT,"Front face"},
	{OBJ_Viewer::SKYBOX_FACE_BACK,"Back face"}
};

inline const std::unordered_map<ImageResolutionEnum_, const char *> kUI_ResolutionOptionLabelMap =
{
	 {ImageResolutionEnum_::ImageResolutionEnum_k_640_X_480, "SD 640 x 480"},
	 {ImageResolutionEnum_::ImageResolutionEnum_k_1280_X_720, "HD 1280 x 720"},
	 {ImageResolutionEnum_::ImageResolutionEnum_k_2560_X_1440,"2K 2560 x 1440"},
	 {ImageResolutionEnum_::ImageResolutionEnum_k_3840_X_2160,"4K 3840 x 2160"},
	 {ImageResolutionEnum_::ImageResolutionEnum_kCustom,"Custom"},

};
inline const std::unordered_map<OBJ_Viewer::ImageFileFormat_, const char*> kUI_ImageFormatLabelMap =
{
	{OBJ_Viewer::ImageFileFormat_::ImageFileFormat_kPNG, "PNG"},
	{OBJ_Viewer::ImageFileFormat_::ImageFileFormat_kJPEG ,"JPEG"},
	{OBJ_Viewer::ImageFileFormat_::ImageFileFormat_kBMP ,"BMP"},

};
#pragma endregion

OBJ_Viewer::UILayer::UILayer(Application& appState,std::shared_ptr<RenderingMediator> renderingMediator):
	m_application(appState),
	m_UI_inputFramebuffer(appState.GetSceneViewport().GetViewportSize(), FramebufferAttachmentsFlags_kColorAttachment)
{
	//m_appEventCallback = m_application.GetOnAppEventCallback();

	this->m_imgGuiDockSpaceFlags = ImGuiDockNodeFlags_None;
	this->m_imGuiWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration;

	m_mediator = renderingMediator;

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

void OBJ_Viewer::UILayer::RenderUI()
{
#pragma region Setup

	auto& pSettings = m_application.GetScene_RefSettings();
	std::shared_ptr<Model> SceneModel = m_mediator->GetModel().lock();

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation = { 0,0,0 };
	SceneModel->GetMatrixDecomposed(position, rotation, scale);
	glm::vec3 previousScale = scale;

	uint32_t vertexCount = 4050, triangleCount = 2323, faceCount = 23232;

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
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open file"))
			{
				EventOnModelLoaded e;
				m_application.SubmitEvent(e);
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
			ImGui::Checkbox("Disable fbx file imports", &pSettings.m_disableFBXLoading);
			ImGui::SetItemTooltip("Due to security vulnerability fbx files are disabled. Enable them on you own risk or if the model is from trusted source");
			ImGui::Separator();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
#pragma endregion

#pragma region Screenshot window
	if (isScreenshotSettingsWindowOpen) {
		if (ImGui::Begin(APP_FOCUS_REGIONS::kUI_WindowScreenshotSettings,&isScreenshotSettingsWindowOpen,ImGuiWindowFlags_NoDocking))
		{
			isAppWindowFocused(APP_FOCUS_REGIONS::kUI_WindowScreenshotSettings);
			static bool renderWithGrid{};
			static bool renderWithTransparency{};
			static ImageFileFormat_ currentlySelectedFormat = ImageFileFormat_::ImageFileFormat_kPNG;
			static ImageResolutionEnum_ currentResolution = ImageResolutionEnum_::ImageResolutionEnum_k_2560_X_1440;
			Size2D imageSize;

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

			currentlySelectedFormat = RenderComboBox("Format", kUI_ImageFormatLabelMap, currentlySelectedFormat);
			currentResolution = RenderComboBox("Image resolution", kUI_ResolutionOptionLabelMap, currentResolution);

			imageSize = GetResolutionFromEnum(currentResolution);
			if (currentResolution == ImageResolutionEnum_::ImageResolutionEnum_kCustom)
			{
				static Size2D tempResolution{};
				ImGui::Text("Custom resolution");
				ImGui::SameLine();
				ImGui::InputInt2("## Texture Size", &tempResolution[0]);
				imageSize = tempResolution;
			}

			ImGui::Checkbox("Render object only", &renderWithGrid);
			ImGui::SetItemTooltip("Don't render the grid and/or skybox.");

			ImGui::Checkbox("Export with transparency", &renderWithTransparency);
			ImGui::SetItemTooltip("If the file format support it will export the scene with transparency.");

			if(ImGui::Button("Take screenshot"))
			{
				DialogWrapper dialogFolderPath;
				dialogFolderPath.OpenDialogSavePath(TextureFileEnumConverter::GetStringTextureFormatFromEnum(currentlySelectedFormat).data());
				if (!dialogFolderPath.IsDialogClosed())
				{
					ImgOutputData eventData{};
					eventData.imgSize = imageSize;
					eventData.outPath = dialogFolderPath.GetDialogResult().at(0);
					eventData.renderObjectOnly = renderWithGrid;
					eventData.outImgFormat = currentlySelectedFormat;
					eventData.allowTransparency = renderWithTransparency;
					ScreenshotEvent e(eventData);
					m_application.SubmitEvent(e);
				}
			}
		}ImGui::End();
	}
#pragma endregion

#pragma region ModelAndRenderingSettings
	//Right panel for model and rendering settings.
	if (ImGui::Begin(APP_FOCUS_REGIONS::kUI_Model_and_renderingSettingsWindowName, (bool*)0, APP_WINDOW_STYLE_FLAGS))
	{
		isAppWindowFocused(APP_FOCUS_REGIONS::kUI_Model_and_renderingSettingsWindowName);

		ImGui::Text("Model view settings");
		ImGui::InputFloat3("Position", &position[0]);
		ImGui::InputFloat3("Rotation", &rotation[0]);
		ImGui::InputFloat3("Scale", &scale[0]);
		ImGui::Checkbox("Use uniform scale", &pSettings.m_isUniformScale);

		if (pSettings.m_isUniformScale)
		{
			float scaleValue = scale.x != previousScale.x ? scale.x :
				scale.y != previousScale.y ? scale.y : scale.z;
			scale = glm::vec3(scaleValue);
		}

		ImGui::Separator();
		if (ImGui::BeginChild("Rendering modes"))
		{
#pragma region Rendering Modes 

			if (ImGui::RadioButton("Wireframe",
				pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe))
			{
				pSettings.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe;
			}
			ImGui::SetItemTooltip("Renders wireframe of the 3D model.");

			if (ImGui::RadioButton("Normal orientation",
				pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kNormalOrientation))
			{
				pSettings.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kNormalOrientation;
			}
			ImGui::SetItemTooltip("Color each face based on the orientation of the normal red if its inwards and blue if outwards.");

			if (ImGui::RadioButton("Clear color rendering",
				pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor))
			{
				pSettings.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor;
			}
			ImGui::SetItemTooltip("Renders the mesh using a single color.");

			if (ImGui::RadioButton("Individual textures",
				pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture))
			{
				pSettings.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture;
			}
			ImGui::SetItemTooltip("Renders only single selected texture.");

			if (ImGui::RadioButton("Uv mode",
				pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kUV))
			{
				pSettings.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kUV;
			}
			ImGui::SetItemTooltip("Renders checkerboard texture for UV inspection.");

			if (ImGui::RadioButton("Light rendering",
				pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode_::RenderingMode_kLight))
			{
				pSettings.m_currentRenderingMode = APP_SETTINGS::RenderingMode_::RenderingMode_kLight;
			}
			ImGui::SetItemTooltip("Render the 3D object with light calculations.");
#pragma endregion

			ImGui::Separator();
			ImGui::Text("Mode settings.");

			switch (pSettings.m_currentRenderingMode)
			{
			case APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe:
				ImGui::InputFloat("Wire line Thickness", &pSettings.wireframeSettings.lineThickness);
				ImGui::ColorPicker3("Line color", &(pSettings.wireframeSettings.lineColor)[0]);
				ImGui::Checkbox("Render points", &pSettings.wireframeSettings.isPointRenderingOn);
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kLight:
				static bool isAlbedoOn = pSettings.m_MaterialFlags & IS_ALBEDO_ON;
				static bool isNormalOn = pSettings.m_MaterialFlags & IS_CUSTOM_SPECULAR_ON;
				static bool isRoughnessOn = pSettings.m_MaterialFlags & IS_CUSTOM_NORMALS_ON;
				static bool isAmbientOcclusionOn = pSettings.m_MaterialFlags & IS_AMBIENT_OCCLUSION_ON;

				ImGui::Checkbox("Albedo?", &isAlbedoOn);
				ImGui::SetItemTooltip("Should the model display with the albedo/color texture.");

				ImGui::Checkbox("Specular?", &isRoughnessOn);
				ImGui::SetItemTooltip("Should the model display with the reflective/specular texture.");

				ImGui::Checkbox("Normals?", &isNormalOn);
				ImGui::SetItemTooltip("Should the model display with the normal map(apply on light calculations).");

				ImGui::Checkbox("Ambient occlusion?", &isAmbientOcclusionOn);
				ImGui::SetItemTooltip("Should the model display with the ambient occlusion texture.");

				pSettings.m_MaterialFlags = FLAGS_NONE;
				pSettings.m_MaterialFlags = isAlbedoOn ? static_cast<MaterialFlags>(pSettings.m_MaterialFlags | IS_ALBEDO_ON) : pSettings.m_MaterialFlags;
				pSettings.m_MaterialFlags = isNormalOn ? static_cast<MaterialFlags>(pSettings.m_MaterialFlags | IS_CUSTOM_NORMALS_ON) : pSettings.m_MaterialFlags;
				pSettings.m_MaterialFlags = isRoughnessOn ? static_cast<MaterialFlags>(pSettings.m_MaterialFlags | IS_CUSTOM_SPECULAR_ON) : pSettings.m_MaterialFlags;
				pSettings.m_MaterialFlags = isAmbientOcclusionOn ? static_cast<MaterialFlags>(pSettings.m_MaterialFlags | IS_AMBIENT_OCCLUSION_ON) : pSettings.m_MaterialFlags;
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture:
				static bool selectedTexture;

				if (ImGui::RadioButton("Color texture",
					pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_ALBEDO))
				{
					pSettings.m_curentIndividualTexture = MATERIAL_TEXTURE_ALBEDO;
				}
				if (ImGui::RadioButton("Normal texture",
					pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_NORMAL))
				{
					pSettings.m_curentIndividualTexture = MATERIAL_TEXTURE_NORMAL;
				}
				if (ImGui::RadioButton("Specular/Metallic texture",
					pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_ROUGHNESS_METALLIC))
				{
					pSettings.m_curentIndividualTexture = MATERIAL_TEXTURE_ROUGHNESS_METALLIC;
				}
				if (ImGui::RadioButton("Ambient occlusion",
					pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_AMBIENT_OCCLUSION))
				{
					pSettings.m_curentIndividualTexture = MATERIAL_TEXTURE_AMBIENT_OCCLUSION;
				}

				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kUV:
				ImGui::SliderFloat("Uv scale", &pSettings.m_uvViewSettings.UV_scaleFactor, 1.f, 150.f);
				break;

			case APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor:
				ImGui::ColorPicker3("Mesh color", &pSettings.m_colorRenderingColor[0]);
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
		
		ModelData currentModelData = SceneModel->GetModelData();

		if (ImGui::BeginTabBar("Tab Scene materials"))
		{

			if (ImGui::BeginTabItem("Scene settings"))
			{
				ImGui::Text("Scene settings.");
				ImGui::Separator();
#pragma region Grid settings
				if (ImGui::CollapsingHeader("World grid settings."))
				{
					ImGui::Checkbox("UseWorldGrid?", &pSettings.m_isWireGridOn);
					if (pSettings.m_isWireGridOn)
					{
						ImGui::SliderFloat("Grid scale", &pSettings.m_gridData.gridScale, 1.f, 10.f);
						ImGui::ColorPicker4("Grid color", &pSettings.m_gridData.gridLineColor[0]);
						ImGui::Checkbox("Shade axis.", &pSettings.m_gridData.isAxisShaded);
					}
				}
#pragma endregion

				ImGui::Separator();
#pragma region Light settings
				if (ImGui::CollapsingHeader("Light settings."))
				{

					static std::vector<const char*> shadingModes = { "Bling-Phong light shading","Toon light shading", "Rim light shading","Rim + toon shading" };
					static const char* currentShadingModel = shadingModes[pSettings.lightInfo.currentLightModel];
					if (ImGui::BeginCombo("Shading mode", currentShadingModel))
					{
						for (int n = 0; n < shadingModes.size(); n++)
						{
							bool is_selected = (currentShadingModel == shadingModes[n]);
							if (ImGui::Selectable(shadingModes[n], is_selected))
							{
								currentShadingModel = shadingModes[n];
								pSettings.lightInfo.currentLightModel = static_cast<APP_SETTINGS::LightShadingModel_>(n);
								break;
							}
						}
						ImGui::EndCombo();
					}

					ImGui::InputInt("Light count", &pSettings.lightInfo.lightCount);
					//If user go beyond 'MAX_LIGHT_COUNT' we use this formula to restrict it.
					/*Basically we have 4(as an example) as out max if we overshoot and go to 5 the expresion "MAX_LIGHT_COUNT - pSettings->lightInfo.lightCount"
						will return negative value, a value that we can use to get the closest valid number in cases of 7 we get 7 += 4 - 7 <=> 7+=-3
						and we use the min function in cases that we are within our range.We do it this way to avoid branching.
						UPDATE: Added this for values below 0 as well by checking the value of 'lightCount' if its below zero we gonna add the positive version of it.
						*/

					pSettings.lightInfo.lightCount =
						pSettings.lightInfo.lightCount < 0 ? (pSettings.lightInfo.lightCount - pSettings.lightInfo.lightCount) :
						pSettings.lightInfo.lightCount + std::min(0, APP_SETTINGS::MAX_LIGHT_COUNT - pSettings.lightInfo.lightCount);
					for (uint32_t i = 0; i < APP_SETTINGS::MAX_LIGHT_COUNT; i++)
					{
						if (i < pSettings.lightInfo.lightCount)
						{
							if (ImGui::CollapsingHeader(std::string("Light " + std::to_string(i + 1)).c_str()))
							{
								RenderLightSettingsPanel(i,
									&pSettings.lightInfo.lights[i].color,
									&pSettings.lightInfo.lights[i].direction);
							}
						}
						else
						{
							pSettings.lightInfo.lights[i].color = glm::vec3(0);
						}
					}
				}
#pragma endregion

				ImGui::Separator();

#pragma region Skybox settings
				if (ImGui::CollapsingHeader("Skybox settings."))
				{
					ImGui::Checkbox("Enable skybox?", &pSettings.m_isSkyboxOn);
					if (pSettings.m_isSkyboxOn)
					{
						RenderSkyboxSettings();
					}
				}
#pragma endregion

				ImGui::Separator();

#pragma region Camera projection and AA settings

				if (ImGui::Button("Switch camera projection"))
				{
					pSettings.isCurrentProjectionPerspective = !pSettings.isCurrentProjectionPerspective;
					EventCameraProjectionChanged e(pSettings.isCurrentProjectionPerspective);
					m_application.SubmitEvent(e);
				}

				ImGui::SetItemTooltip("Current projection mode is:%s", pSettings.isCurrentProjectionPerspective ? "Perspective" : "Orthographic");
				ImGui::Checkbox("Enable AA", &pSettings.m_EnableAA);
				ImGui::SetItemTooltip("Enable scene Anti-aliasing(AA).The scene AA is using MSAA + FXAA");
#pragma endregion

				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("Scene/Model info"))
			{
#pragma region Scene mesh info
				ImGui::Text("Model info");
				ImGui::Text("Object triangle count:%d", currentModelData.meshInfo.triangleCount);
				ImGui::Text("Object vertex count:%d", currentModelData.meshInfo.vertexCount);
				ImGui::Text("Object face count:%d", currentModelData.meshInfo.faceCount);
				ImGui::Text("Texture count:%d", currentModelData.textureCount);

				ImGui::Text("File path %s", currentModelData.modelPath.c_str());
				ImGui::SetItemTooltip("Path:%s", currentModelData.modelPath.c_str());

				ImGui::Separator();
#pragma endregion

#pragma region Render scene materials
				if (const auto registry = m_mediator->GetMaterialRegistry().lock())
				{
					ImGui::Text("Scene materials");

					for (size_t i = 0; i < registry->GetMaterialCount(); i++)
					{
						if (const auto& material = registry->GetMaterialAtIndex(i).lock())
						{
							if (ImGui::CollapsingHeader(material->GetMaterialName().c_str()))
							{
								//Add imported textures preview

								RenderMaterial_LabelTexturePair(material, MaterialTextures::MATERIAL_TEXTURE_ALBEDO, "Albedo");

								RenderMaterial_LabelTexturePair(material, MaterialTextures::MATERIAL_TEXTURE_NORMAL, "Normal");

								RenderMaterial_LabelTexturePair(material, MaterialTextures::MATERIAL_TEXTURE_ROUGHNESS_METALLIC, "Specular");

								RenderMaterial_LabelTexturePair(material, MaterialTextures::MATERIAL_TEXTURE_AMBIENT_OCCLUSION,
									"Ambient occlusion");
							}
						}
					}
					ImGui::Separator();
				}
#pragma endregion

#pragma region  Render scene hierarchy
				ImGui::Text("Scene hierarchy");
				constexpr size_t SCENE_OBJECT_COUNT_TEST = 5;
				for (size_t i = 0; i < SCENE_OBJECT_COUNT_TEST; i++)
				{
					ImGui::Text("Name of mesh");
					ImGui::SameLine();
					ImGui::PushItemWidth(-1);
					if (ImGui::RadioButton(std::string("##" + std::to_string(i + 1)).c_str(), true))
					{
						LOGGER_LOG_INFO("OBJECT_VISIBILITY IS CHANGED");
					}
					ImGui::PopItemWidth();
				}
				ImGui::EndTabItem();
			}
#pragma endregion

		}ImGui::EndTabBar();
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
		m_application.SubmitSceneViewportSize(sceneWinViewport);

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

	m_application.GetGlobalInputHandler().SetCurrentlyFocusedWindow(m_currentlyFocusedWindow);

	SceneModel->ApplyTransformation(position, scale, glm::vec3(1), 0);
}

void OBJ_Viewer::UILayer::RenderMaterial_LabelTexturePair(const std::shared_ptr<OBJ_Viewer::Material>& material,
	MaterialTextures textureType,const char* textureLabelName)
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

void OBJ_Viewer::UILayer::RenderSkyboxSettings()
{
	static 
	int SelectedItemTemp;
	ImGui::Separator();
	
	if (ImGui::Button("Load skybox textures."))
	{
		
		EventOnSkyboxLoaded e;
		m_application.SubmitEvent(e);		
	}
	if (auto sceneSkybox = m_mediator->GetSkybox().lock())
	{
		std::vector <std::shared_ptr<Texture>> skyboxTextures = sceneSkybox->GetSkyboxFaceTextures();

		int noTextureLoaded = 0;
		static SkyboxFace comboResult{};
		static std::string index_to_string = "## ";
		for (uint8_t i = 0; i < Skybox::SKYBOX_FACE_COUNT; i++)
		{
			ImGui::Image((ImTextureID)skyboxTextures[i]->GetTextureHandle(),{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			ImGui::SameLine();
			index_to_string[index_to_string.size() - 1] = '0' + i;
			comboResult = RenderComboBox<SkyboxFace>(index_to_string, kUI_SkyboxFaceLabelMap,
				static_cast<SkyboxFace>(i));

			if(i != comboResult)
				m_mediator->GetSkybox().lock()->SwapSkyboxFaceTextures(static_cast<SkyboxFace>(i), comboResult);
		}

		//TODO:Add texture preview;
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
