#include "pch.h"
#include "UILayer.h"
#include"Helpers/DialogWrapper.h"

inline std::vector<std::string> itemsLabel = { "Right face" ,"Left face","Top face","Bottom face","Front face","Back face" };
inline std::vector<OBJ_Viewer::SkyboxFace> itemsFaces = {
		OBJ_Viewer::SKYBOX_FACE_RIGHT,
		OBJ_Viewer::SKYBOX_FACE_LEFT,
		OBJ_Viewer::SKYBOX_FACE_TOP,
		OBJ_Viewer::SKYBOX_FACE_BOTTOM,
		OBJ_Viewer::SKYBOX_FACE_FRONT,
		OBJ_Viewer::SKYBOX_FACE_BACK};

OBJ_Viewer::UILayer::UILayer(Application& appState,
	std::shared_ptr<RenderingMediator> renderingMediator,
	ImGuiWindowFlags imGuiWindowFlags,
	ImGuiDockNodeFlags imGuiDockSpaceFlags):m_application(appState)
{
	m_appEventCallback = m_application.GetOnAppEventCallback();

	this->m_imgGuiDockSpaceFlags = imGuiDockSpaceFlags;
	this->m_imGuiWindowFlags = imGuiWindowFlags;
	m_mediator = renderingMediator;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (this->m_imgGuiDockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoBackground;
}

void OBJ_Viewer::UILayer::RenderUI()
{
#pragma region Setup

	auto& pSettings = m_application.GetScene_RefSettings();
	std::shared_ptr<Model> SceneModel = m_mediator->GetModel().lock();
	const Framebuffer& sceneFrameBuffer = m_application.GetSceneFrameBuffer();
	const char* currentlyActiveWindow = APP_FOCUS_REGIONS::UI_WINDOW_UNKNOWN;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();	
	
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation = { 0,0,0 };
	SceneModel->GetMatrixDecomposed(position, rotation, scale);
	glm::vec3 previousScale = scale;

	uint32_t vertexCount = 4050, triangleCount = 2323, faceCount = 23232;

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

#pragma region ModelAndRenderingSettings


	//Right panel for model and rendering settings.
	if (ImGui::Begin(APP_FOCUS_REGIONS::UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)? 
			APP_FOCUS_REGIONS::UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME : currentlyActiveWindow;

		ImGui::Text("Model view settings");
		ImGui::InputFloat3("Position", &position[0]);
		ImGui::InputFloat3("Rotation", &rotation[0]);
		ImGui::InputFloat3("Scale", &scale[0]);
		ImGui::Checkbox("Use uniform scale", &pSettings.m_isUniformScale);
		if (pSettings.m_isUniformScale)
		{
			float scaleValue = scale.x != previousScale.x ? scale.x:
				scale.y != previousScale.y ? scale.y : scale.z;
			scale = glm::vec3(scaleValue);
		}
		ImGui::Separator();
		if (ImGui::BeginChild("Rendering modes"))
		{
			ImGui::Text("Model rendering modes.");
			static bool renderingModeContainer;
			ImGui::Checkbox("Wireframe",
				&(renderingModeContainer = pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode::RENDER_MODE_WIREFRAME));
			ImGui::SetItemTooltip("Renders wireframe of the 3D model.");
			pSettings.m_currentRenderingMode = renderingModeContainer ? APP_SETTINGS::RenderingMode::RENDER_MODE_WIREFRAME :
				pSettings.m_currentRenderingMode;

			ImGui::Checkbox("Clear color rendering.",
				&(renderingModeContainer = pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode::RENDER_MODE_SOLID_COLOR));
			ImGui::SetItemTooltip("Renders the mesh using a single color.");
			pSettings.m_currentRenderingMode = renderingModeContainer ? APP_SETTINGS::RenderingMode::RENDER_MODE_SOLID_COLOR :
				pSettings.m_currentRenderingMode;

			ImGui::Checkbox("Individual textures",
				&(renderingModeContainer = pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode::RENDER_MODE_INDIVIDUAL_TEXTURES));
			ImGui::SetItemTooltip("Renders only single selected texture.");
			pSettings.m_currentRenderingMode = renderingModeContainer ? APP_SETTINGS::RenderingMode::RENDER_MODE_INDIVIDUAL_TEXTURES :
				pSettings.m_currentRenderingMode;

			ImGui::Checkbox("Uv mode",
				&(renderingModeContainer = pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode::RENDER_MODE_UV));
			ImGui::SetItemTooltip("Renders checkerboard texture for UV inspection.");
			pSettings.m_currentRenderingMode = renderingModeContainer ? APP_SETTINGS::RenderingMode::RENDER_MODE_UV :
				pSettings.m_currentRenderingMode;

			ImGui::Checkbox("Light rendering",
				&(renderingModeContainer = pSettings.m_currentRenderingMode == APP_SETTINGS::RenderingMode::RENDER_MODE_LIGHT));
			ImGui::SetItemTooltip("Render the 3D object with light calculations.");
			pSettings.m_currentRenderingMode = renderingModeContainer ? APP_SETTINGS::RenderingMode::RENDER_MODE_LIGHT :
				pSettings.m_currentRenderingMode;

			ImGui::Separator();
			ImGui::Text("Mode settings.");
			switch (pSettings.m_currentRenderingMode)
			{
			case APP_SETTINGS::RenderingMode::RENDER_MODE_WIREFRAME:
				ImGui::InputFloat("Wire line Thickness", &pSettings.wireframeSettings.lineThickness);
				ImGui::ColorPicker3("Line color", &(pSettings.wireframeSettings.lineColor)[0]);
				ImGui::Checkbox("Render points", &pSettings.wireframeSettings.isPointRenderingOn);
				break;
	
			case APP_SETTINGS::RenderingMode::RENDER_MODE_LIGHT:
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
				pSettings.m_MaterialFlags = isNormalOn ? static_cast<MaterialFlags>(pSettings.m_MaterialFlags | IS_CUSTOM_NORMALS_ON): pSettings.m_MaterialFlags;
				pSettings.m_MaterialFlags = isRoughnessOn ? static_cast<MaterialFlags>(pSettings.m_MaterialFlags | IS_CUSTOM_SPECULAR_ON) : pSettings.m_MaterialFlags;
				pSettings.m_MaterialFlags = isAmbientOcclusionOn ? static_cast<MaterialFlags>(pSettings.m_MaterialFlags | IS_AMBIENT_OCCLUSION_ON) : pSettings.m_MaterialFlags;
				break;

			case APP_SETTINGS::RenderingMode::RENDER_MODE_INDIVIDUAL_TEXTURES:
				static bool selectedTexture;

				ImGui::Checkbox("Color texture", &(selectedTexture = pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_ALBEDO));
				pSettings.m_curentIndividualTexture = selectedTexture ? MATERIAL_TEXTURE_ALBEDO : pSettings.m_curentIndividualTexture;

				ImGui::Checkbox("Normal texture", &(selectedTexture = pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_NORMAL));
				pSettings.m_curentIndividualTexture = selectedTexture ? MATERIAL_TEXTURE_NORMAL : pSettings.m_curentIndividualTexture;

				ImGui::Checkbox("Specular/Metallic texture", &(selectedTexture = pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_ROUGHNESS_METALLIC));
				pSettings.m_curentIndividualTexture = selectedTexture ? MATERIAL_TEXTURE_ROUGHNESS_METALLIC : pSettings.m_curentIndividualTexture;

				ImGui::Checkbox("Ambient occlusion", &(selectedTexture = pSettings.m_curentIndividualTexture == MATERIAL_TEXTURE_AMBIENT_OCCLUSION));
				pSettings.m_curentIndividualTexture = selectedTexture ? MATERIAL_TEXTURE_AMBIENT_OCCLUSION : pSettings.m_curentIndividualTexture;
				break;

			case APP_SETTINGS::RenderingMode::RENDER_MODE_UV:
				ImGui::SliderFloat("Uv scale", &pSettings.m_uvViewSettings.UV_scaleFactor, 1.f, 150.f);
				break;

			case APP_SETTINGS::RenderingMode::RENDER_MODE_SOLID_COLOR:
				//TODO:Implement
				ImGui::ColorPicker3("Mesh color", &pSettings.m_colorRenderingColor[0]);
				break;
			default:
				break;
			}
		}ImGui::EndChild();
	}ImGui::End();
#pragma endregion

#pragma region Scene settings


	if (ImGui::Begin(APP_FOCUS_REGIONS::UI_LAYER_SCENE_SETTINGS_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			APP_FOCUS_REGIONS::UI_LAYER_SCENE_SETTINGS_WINDOW_NAME : currentlyActiveWindow;
		ModelData currentModelData = SceneModel->GetModelData();
#pragma region Object info


		ImGui::Text("Object triangle count:%d", currentModelData.triangleCount);
		ImGui::Text("Object vertex count:%d", currentModelData.vertexCount);
		ImGui::Text("Object face count:%d", currentModelData.faceCount);
		ImGui::Text("Texture count:%d", currentModelData.textureCount);

		ImGui::Text("File path %s", currentModelData.modelPath.c_str());
		ImGui::SetItemTooltip("Path:%s", currentModelData.modelPath.c_str());

		ImGui::Separator();
#pragma endregion

#pragma region Grid settings


		ImGui::Text("Scene settings.");
		ImGui::Separator();
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
		ImGui::Separator();
#pragma endregion

#pragma region  Light settings


		if (ImGui::CollapsingHeader("Light settings."))
		{
	
			static std::vector<const char*> shadingModes = {"Bling-Phong light shading","Toon light shading", "Rim light shading","Rim + toon shading"};
			static const char* currentShadingModel = shadingModes[pSettings.lightInfo.currentLightModel];
			if (ImGui::BeginCombo("Shading mode", currentShadingModel)) 
			{
				for (int n = 0; n < shadingModes.size(); n++)
				{
					bool is_selected = (currentShadingModel == shadingModes[n]);
					if (ImGui::Selectable(shadingModes[n], is_selected))
					{
						currentShadingModel = shadingModes[n];
						pSettings.lightInfo.currentLightModel = static_cast<APP_SETTINGS::LightShadingModel>(n);
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
					if (ImGui::CollapsingHeader(std::string("Light " + std::to_string(i+1)).c_str()))
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
		ImGui::Separator();
#pragma endregion

#pragma region Rest of settings


		if (ImGui::CollapsingHeader("Skybox settings."))
		{
			ImGui::Checkbox("Enable skybox?", &pSettings.m_isSkyboxOn);
			if (pSettings.m_isSkyboxOn)
			{
				RenderSkyboxSettings();
			}
		}
		ImGui::Separator();
		if (ImGui::Button("Switch camera projection"))
		{
			pSettings.isCurrentProjectionPerspective = !pSettings.isCurrentProjectionPerspective;
			EventCameraProjectionChanged e(pSettings.isCurrentProjectionPerspective);
			m_appEventCallback(e);
		}
		ImGui::SetItemTooltip("Current projection mode is:%s", pSettings.isCurrentProjectionPerspective ? "Perspective" : "Orthographic");
		ImGui::Checkbox("Enable AA", &pSettings.m_EnableAA);
		ImGui::SetItemTooltip("Enable scene Anti-aliasing(AA).The scene AA is using MSAA + FXAA");
	}ImGui::End();
#pragma endregion

#pragma endregion

#pragma region Loading object info


	if(ImGui::Begin(APP_FOCUS_REGIONS::UI_LAYER_OBJECT_LOADING_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			APP_FOCUS_REGIONS::UI_LAYER_OBJECT_LOADING_WINDOW_NAME : currentlyActiveWindow;
	ImGui::Text("Loading stuff here.");
	}ImGui::End();
	
	if (ImGui::Begin(APP_FOCUS_REGIONS::UI_LAYER_OBJECT_LOADING_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			APP_FOCUS_REGIONS::UI_LAYER_OBJECT_LOADING_WINDOW_NAME : currentlyActiveWindow;

		if (ImGui::Button("Import 3D model."))
		{
			LoadModel();
		}
		ImGui::SameLine();
		ImGui::Checkbox("Disable fbx file imports",&pSettings.m_disableFBXLoading);
		ImGui::SetItemTooltip("Due to security vulnerability fbx files are disabled. Enable them on you own risk or if the model is from trusted source");
		ImGui::Separator();

		ImGui::Text("Scene material textures.");
		//Add imported textures preview
		
		static constexpr float spacing = 50.f;
		static const ImVec2 textSize = { 70,70 };
		ImGui::Text("Albedo");
		ImGui::SameLine(0, spacing);
		ImGui::Text("Normal");
		ImGui::SameLine(0, spacing);
		ImGui::Text("Specular");
		ImGui::SameLine(0, spacing);
		ImGui::Text("Ambient Occlusion");
		
		ImGui::Image(0,
			textSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine(0, spacing);
		ImGui::Image(0,
			textSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine(0, spacing);
		ImGui::Image(0,
			textSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine(0, spacing);
		ImGui::Image(0,
			textSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine(0, spacing);

	}ImGui::End();
#pragma endregion

#pragma region Scene window


	if(ImGui::Begin(APP_FOCUS_REGIONS::UI_LAYER_SCENE_WINDOW_NAME,(bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			APP_FOCUS_REGIONS::UI_LAYER_SCENE_WINDOW_NAME : currentlyActiveWindow;

		ImVec2 winSize = ImGui::GetWindowSize();
		ImVec2 winPos = ImGui::GetWindowPos();
		static Viewport sceneWinViewport{};
		sceneWinViewport.x = winPos.x;
		sceneWinViewport.y = winPos.y;
		sceneWinViewport.width = winSize.x;
		sceneWinViewport.height = winSize.y;

		m_application.UpdateSceneViewport(sceneWinViewport);
		ImGui::BeginChild("GameRender", winSize,ImGuiChildFlags_None, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::Image((ImTextureID)sceneFrameBuffer.GetFramebufferHandle(), winSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::EndChild();

	}ImGui::End();
#pragma endregion

#pragma region Submit UI

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion
	m_application.GetGlobalInputHandler().SetCurrentlyFocusedWindow(currentlyActiveWindow);

	SceneModel->ApplyTransformation(position, scale, glm::vec3(1), 0);
}

void OBJ_Viewer::UILayer::RenderSkyboxSettings()
{
	static 
	int SelectedItemTemp;
	ImGui::Separator();
	
	if (ImGui::Button("Load skybox textures."))
	{
		LoadSkybox();
		itemsFaces = {
		OBJ_Viewer::SKYBOX_FACE_RIGHT,
		OBJ_Viewer::SKYBOX_FACE_LEFT,
		OBJ_Viewer::SKYBOX_FACE_TOP,
		OBJ_Viewer::SKYBOX_FACE_BOTTOM,
		OBJ_Viewer::SKYBOX_FACE_FRONT,
		OBJ_Viewer::SKYBOX_FACE_BACK };
	}
	std::shared_ptr<Skybox> sceneSkybox;
	bool isSkyboxExpired = m_mediator->GetSkybox().expired();
	if (!isSkyboxExpired)
	{
		sceneSkybox = m_mediator->GetSkybox().lock();
		std::vector <std::shared_ptr<Texture>> skyboxTextures = sceneSkybox->GetSkyboxFaceTextures();

		int noTextureLoaded = 0;
		//Replace this with checkerboard texture.
		ImGui::Image(!isSkyboxExpired ? (ImTextureID)skyboxTextures[0]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
			{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		RenderComboBox("0", 0);

		ImGui::Image(!isSkyboxExpired ? (ImTextureID)skyboxTextures[1]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
			{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		RenderComboBox("1", 1);

		ImGui::Image(!isSkyboxExpired ? (ImTextureID)skyboxTextures[2]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
			{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		RenderComboBox("2", 2);

		ImGui::Image(!isSkyboxExpired ? (ImTextureID)skyboxTextures[3]->GetTextureHandle() : (ImTextureID)noTextureLoaded
			, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		RenderComboBox("3", 3);

		ImGui::Image(!isSkyboxExpired ? (ImTextureID)skyboxTextures[4]->GetTextureHandle() : (ImTextureID)noTextureLoaded
			, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		RenderComboBox("4", 4);

		ImGui::Image(!isSkyboxExpired ? (ImTextureID)skyboxTextures[5]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
			{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		RenderComboBox("5", 5);

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

void OBJ_Viewer::UILayer::RenderComboBox(std::string comboLabel, int index)
{	
	if (ImGui::BeginCombo(comboLabel.c_str(), itemsLabel[index].c_str())) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < itemsLabel.size(); n++)
		{
			bool is_selected = (itemsLabel[index] == itemsLabel[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(itemsLabel[n].c_str(), is_selected))
			{	
				m_mediator->GetSkybox().lock()->SwapSkyboxFaceTextures(itemsFaces[index], itemsFaces[n]);
				break;
			}
		}
		ImGui::EndCombo();
	}
}

void OBJ_Viewer::UILayer::LoadModel()
{
	DialogWrapper dialog;
	std::string filterList = "obj";

	if (!m_application.GetScene_RefSettings().m_disableFBXLoading)
		filterList += ",fbx";

	dialog.OpenDialog(filterList);
	//If error occurs or the user change their mind we wont send an event;
	if (dialog.isDialogClosed())
		return;

	auto VecPaths = dialog.GetDialogResult();
	EventOnModelLoaded e(std::string(VecPaths.at(0)));
	m_appEventCallback(e);
}

void OBJ_Viewer::UILayer::LoadSkybox()
{
	DialogWrapper dialog;
	dialog.OpenDialogMultiple("png,jpeg,jpg");

	//If error occurs or the user change their mind we wont send an event;
	if (dialog.isDialogClosed())
		return;

	auto VecPaths = dialog.GetDialogResult();
	std::vector<std::string> m_stringVector(VecPaths.size());
	for (uint32_t i = 0; i < VecPaths.size(); i++)
	{
		m_stringVector[i] = std::string(VecPaths.at(i));
	}
	EventOnSkyboxLoaded e(m_stringVector);
	m_appEventCallback(e);
}
