#include "UILayer.h"
#include"DialogWrapper.h"
#include<algorithm>

inline std::vector<std::string> itemsLabel = { "Right face" ,"Left face","Top face","Bottom face","Front face","Back face" };
inline std::vector<OBJ_Viewer::SkyboxFace> itemsFaces ={
		OBJ_Viewer::SKYBOX_FACE_RIGHT,
		OBJ_Viewer::SKYBOX_FACE_LEFT,
		OBJ_Viewer::SKYBOX_FACE_TOP,
		OBJ_Viewer::SKYBOX_FACE_BOTTOM,
		OBJ_Viewer::SKYBOX_FACE_FRONT,
		OBJ_Viewer::SKYBOX_FACE_BACK};

OBJ_Viewer::UILayer::UILayer(AppState* appState,
	ImGuiWindowFlags imGuiWindowFlags,
	ImGuiDockNodeFlags imGuiDockSpaceFlags)
{
	m_appState = appState;

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

	auto* pSettings = m_appState->GetScene_pSettings();
	Model SceneModel = m_appState->GetSceneModel();
	const Framebuffer& sceneFrameBuffer = m_appState->GetSceneFrameBuffer();
	const Skybox* sceneSkybox = m_appState->GetSceneSkybox();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	float xPos = 1.0f;
	glm::vec3 position = { 0,0,0 };
	glm::vec3 scale = { 1,1,1 };
	glm::vec3 rotation = { 0,0,0 };
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
	if (ImGui::Begin("Test"))
	{
		ImGui::Text("Model view settings");
		ImGui::InputFloat3("Position", &position[0]);
		ImGui::InputFloat3("Rotation", &rotation[0]);
		ImGui::InputFloat3("Scale", &scale[0]);
		ImGui::Separator();
		ImGui::Text("Model rendering settings.");
		ImGui::Checkbox("Wireframe?", &pSettings->m_isWireFrameRenderingOn);
		ImGui::Checkbox("Albedo?", &pSettings->m_isRenderAlbedoTextureOn);
		ImGui::Checkbox("Specular?", &pSettings->m_isRenderSpecularTextureOn);
		ImGui::Checkbox("Normals?", &pSettings->m_isRenderNormalTextureOn);
		ImGui::Checkbox("Ambient occlusion?", &pSettings->m_isRenderAmbientOcclusionTextureOn);

	}ImGui::End();

	//ModelData modelData = (*m_pCurrentlyLoadedModel)->GetModelData();
	if (ImGui::Begin("Model data."))
	{
		ImGui::Text("Object triangle count:%d", 1);
		ImGui::Text("Object vertex count:%d", 2);
		ImGui::Text("Object face count:%d", 3);
		ImGui::Text("Texture count:%d", 6);
		ImGui::Text("File path %s", "Dummy path");

		ImGui::Separator();
		ImGui::Text("Scene settings.");
		ImGui::Checkbox("UseWorldGrid?", &pSettings->m_isWireGridOn);
		ImGui::Checkbox("Enable lights?", &pSettings->m_isRenderingLightOn);

		if (pSettings->m_isRenderingLightOn)
		{
			static float color;
			static int lightCount;
			ImGui::InputInt("Light count", &lightCount);
			ImGui::ColorPicker4("Light color", &color);
		}
		ImGui::Checkbox("Enable skybox?", &pSettings->m_isSkyboxOn);
		if (pSettings->m_isSkyboxOn)
		{
			RenderSkyboxSettings(sceneSkybox);
		}

	}ImGui::End();

	ImGui::Begin("Loading panel");
	ImGui::Text("Loading stuff here.");
	ImGui::End();

	if (ImGui::Begin("Loading panel"))
	{
		if (ImGui::Button("Open obj file"))
		{
			LoadModel();
		}
		ImGui::Text("Loading stuff here.");

	}ImGui::End();

	ImGui::Begin("Scene");
	{
		ImVec2 winSize = ImGui::GetWindowSize();
		m_appState->ResizeBuffer(winSize.x, winSize.y);
		ImGui::BeginChild("GameRender");
		ImGui::Image((ImTextureID)sceneFrameBuffer.GetFramebufferHandle(), winSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::EndChild();

	}ImGui::End();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OBJ_Viewer::UILayer::RenderSkyboxSettings(const Skybox* skybox)
{
	static 
	int SelectedItemTemp;
	ImGui::Separator();
	if (ImGui::Button("Load skybox textures."))
	{
		LoadSkybox();
		skybox = m_appState->GetSceneSkybox();
		itemsFaces = {
		OBJ_Viewer::SKYBOX_FACE_RIGHT,
		OBJ_Viewer::SKYBOX_FACE_LEFT,
		OBJ_Viewer::SKYBOX_FACE_TOP,
		OBJ_Viewer::SKYBOX_FACE_BOTTOM,
		OBJ_Viewer::SKYBOX_FACE_FRONT,
		OBJ_Viewer::SKYBOX_FACE_BACK };
	}
	int noTextureLoaded = 0;
	//Replace this with checkerboard texture.
	ImGui::Image(skybox != nullptr ? (ImTextureID)skybox->m_faceTextures[0]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
		{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	ImGui::SameLine();
	RenderComboBox("0", 0);

	ImGui::Image(skybox != nullptr ? (ImTextureID)skybox->m_faceTextures[1]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
		{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	ImGui::SameLine();
	RenderComboBox("1", 1);

	ImGui::Image(skybox != nullptr ? (ImTextureID)skybox->m_faceTextures[2]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
		{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	ImGui::SameLine();
	RenderComboBox("2", 2);

	ImGui::Image(skybox != nullptr ? (ImTextureID)skybox->m_faceTextures[3]->GetTextureHandle() : (ImTextureID)noTextureLoaded
		, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	ImGui::SameLine();
	RenderComboBox("3", 3);

	ImGui::Image(skybox != nullptr ? (ImTextureID)skybox->m_faceTextures[4]->GetTextureHandle() : (ImTextureID)noTextureLoaded
		, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	ImGui::SameLine();
	RenderComboBox("4",4);

	ImGui::Image(skybox != nullptr ? (ImTextureID)skybox->m_faceTextures[5]->GetTextureHandle() : (ImTextureID)noTextureLoaded,
		{ 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	ImGui::SameLine();
	RenderComboBox("5", 5);

	//TODO:Add texture preview;
	ImGui::Separator();
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
				m_appState->SwapSkyboxFaces(itemsFaces[index], itemsFaces[n]);
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
	m_appState->LoadModel(VecPaths[0]);
}

void OBJ_Viewer::UILayer::LoadSkybox()
{
	DialogWrapper dialog;
	dialog.OpenDialogMultiple("png,jpeg,jpg");
	auto VecPaths = dialog.GetDialogResult();
	m_appState->LoadSkybox(VecPaths);

}
