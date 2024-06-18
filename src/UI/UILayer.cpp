#include "UILayer.h"
#include"DialogWrapper.h"
#include<algorithm>
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

inline std::vector<std::string> itemsLabel = { "Right face" ,"Left face","Top face","Bottom face","Front face","Back face" };
inline std::vector<OBJ_Viewer::SkyboxFace> itemsFaces = {
		OBJ_Viewer::SKYBOX_FACE_RIGHT,
		OBJ_Viewer::SKYBOX_FACE_LEFT,
		OBJ_Viewer::SKYBOX_FACE_TOP,
		OBJ_Viewer::SKYBOX_FACE_BOTTOM,
		OBJ_Viewer::SKYBOX_FACE_FRONT,
		OBJ_Viewer::SKYBOX_FACE_BACK};

OBJ_Viewer::UILayer::UILayer(AppState* appState,
	std::shared_ptr<SceneRenderer> sceneRenderer,
	ImGuiWindowFlags imGuiWindowFlags,
	ImGuiDockNodeFlags imGuiDockSpaceFlags)
{
	m_appState = appState;
	this->m_sceneRenderer = sceneRenderer;

	this->m_imgGuiDockSpaceFlags = imGuiDockSpaceFlags;
	this->m_imGuiWindowFlags = imGuiWindowFlags;

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

	auto& pSettings = m_appState->GetScene_RefSettings();
	std::shared_ptr<Model> SceneModel = m_sceneRenderer->GetSceneModel().lock();
	const Framebuffer& sceneFrameBuffer = m_appState->GetSceneFrameBuffer();
	const char* currentlyActiveWindow = UI_WINDOW_UNKNOWN;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	float xPos = 1.0f;
	
	
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation = { 0,0,0 };
	SceneModel->GetMatrixDecomposed(position, rotation, scale);
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
	//Right panel for model and rendering settings.
	if (ImGui::Begin(UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)? 
			UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME : currentlyActiveWindow;

		ImGui::Text("Model view settings");
		ImGui::InputFloat3("Position", &position[0]);
		ImGui::InputFloat3("Rotation", &rotation[0]);
		ImGui::InputFloat3("Scale", &scale[0]);
		ImGui::Separator();
		ImGui::Text("Model rendering settings.");
		ImGui::Checkbox("Wireframe?", &pSettings.m_isWireFrameRenderingOn);
		ImGui::Checkbox("Albedo?", &pSettings.m_isRenderAlbedoTextureOn);
		ImGui::Checkbox("Specular?", &pSettings.m_isRenderSpecularTextureOn);
		ImGui::Checkbox("Normals?", &pSettings.m_isRenderNormalTextureOn);
		ImGui::Checkbox("Ambient occlusion?", &pSettings.m_isRenderAmbientOcclusionTextureOn);
	}ImGui::End();

	//ModelData modelData = (*m_pCurrentlyLoadedModel)->GetModelData();
	if (ImGui::Begin(UI_LAYER_SCENE_SETTINGS_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			UI_LAYER_SCENE_SETTINGS_WINDOW_NAME : currentlyActiveWindow;

		ImGui::Text("Object triangle count:%d", 1);
		ImGui::Text("Object vertex count:%d", 2);
		ImGui::Text("Object face count:%d", 3);
		ImGui::Text("Texture count:%d", 6);
		ImGui::Text("File path %s", "Dummy path");

		ImGui::Separator();
		ImGui::Text("Scene settings.");
		ImGui::Checkbox("UseWorldGrid?", &pSettings.m_isWireGridOn);
		if (pSettings.m_isWireGridOn)
		{
			ImGui::SliderFloat("Grid scale", &pSettings.m_gridData.gridScale, 1.f, 10.f);
			ImGui::ColorPicker4("Grid color", &pSettings.m_gridData.gridLineColor[0]);
			ImGui::Checkbox("Shade axis.", &pSettings.m_gridData.isAxisShaded);
		}
		ImGui::Checkbox("Enable lights?", &pSettings.m_isRenderingLightOn);

		if (pSettings.m_isRenderingLightOn)
		{
			
			ImGui::InputInt("Light count", &pSettings.lightInfo.lightCount);
			//If user go beyond 'MAX_LIGHT_COUNT' we use this formula to restrict it.
			/*Basically we have 4(as an example) as out max if we overshoot and go to 5 the expresion "MAX_LIGHT_COUNT - pSettings->lightInfo.lightCount"
			will return negative value, a value that we can use to get the closest valid number in cases of 7 we get 7 += 4 - 7 <=> 7+=-3
			and we use the min function in cases that we are within our range.We do it this way to avoid branching.
			UPDATE: Added this for values below 0 as well by checking the value of 'lightCount' if its below zero we gonna add the positive version of it.
			*/

			pSettings.lightInfo.lightCount = 
				pSettings.lightInfo.lightCount < 0 ? (pSettings.lightInfo.lightCount - pSettings.lightInfo.lightCount):
				pSettings.lightInfo.lightCount + std::min(0, MAX_LIGHT_COUNT - pSettings.lightInfo.lightCount);

			for (uint32_t i = 0; i < pSettings.lightInfo.lightCount; i++)
			{
				RenderLightSettingsPanel(i, 
					&pSettings.lightInfo.lights[i].color,
					&pSettings.lightInfo.lights[i].direction);
				ImGui::Separator();
			}
		}
		ImGui::Checkbox("Enable skybox?", &pSettings.m_isSkyboxOn);
		if (pSettings.m_isSkyboxOn)
		{
			RenderSkyboxSettings();
		}

	}ImGui::End();

	if(ImGui::Begin(UI_LAYER_OBJECT_LOADING_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			UI_LAYER_OBJECT_LOADING_WINDOW_NAME : currentlyActiveWindow;
	ImGui::Text("Loading stuff here.");
	}ImGui::End();
	
	if (ImGui::Begin(UI_LAYER_OBJECT_LOADING_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			UI_LAYER_OBJECT_LOADING_WINDOW_NAME : currentlyActiveWindow;

		if (ImGui::Button("Open obj file"))
		{
			LoadModel();
		}
		ImGui::Text("Loading stuff here.");

	}ImGui::End();

	if(ImGui::Begin(UI_LAYER_SCENE_WINDOW_NAME))
	{
		currentlyActiveWindow = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) ?
			UI_LAYER_SCENE_WINDOW_NAME : currentlyActiveWindow;

		ImVec2 winSize = ImGui::GetWindowSize();
		m_appState->ResizeBuffer(winSize.x, winSize.y);
		ImGui::BeginChild("GameRender");
		ImGui::Image((ImTextureID)sceneFrameBuffer.GetFramebufferHandle(), winSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::EndChild();

	}ImGui::End();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	m_appState->GetGlobalInputHandler().SetCurrentlyFocusedWindow(currentlyActiveWindow);

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
	bool isSkyboxExpired = m_sceneRenderer->GetSkyboxModel().expired();
	if (!isSkyboxExpired)
	{
		sceneSkybox = m_sceneRenderer->GetSkyboxModel().lock();
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
				m_sceneRenderer->SwapSkyboxFaces(itemsFaces[index], itemsFaces[n]);
				break;
			}
		}
		ImGui::EndCombo();
	}
}

void OBJ_Viewer::UILayer::LoadModel()
{
	DialogWrapper dialog;
	dialog.OpenDialog();
	auto VecPaths = dialog.GetDialogResult();
	m_sceneRenderer->LoadModel(VecPaths[0]);
}

void OBJ_Viewer::UILayer::LoadSkybox()
{
	DialogWrapper dialog;
	dialog.OpenDialogMultiple("png,jpeg,jpg");
	auto VecPaths = dialog.GetDialogResult();
	m_sceneRenderer->LoadSkybox(VecPaths);
}
