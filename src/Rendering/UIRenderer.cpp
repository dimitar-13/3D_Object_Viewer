#include "UIRenderer.h"
#include"RenderingCoordinator.h"
#include"DialogWrapper.h"
OBJ_Viewer::UIRenderer::UIRenderer(ImGuiWindowFlags imGuiWindowFlags,
	ImGuiDockNodeFlags imGuiDockSpaceFlags, RendererSettings* pRendererSettings)
{
	this->m_pRendererSettings = pRendererSettings;

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

//Yes this is pointless.It will be changed in the future but for now i dont think there is a huge point in designing a imGui abstraction.
void OBJ_Viewer::UIRenderer::RenderUI(GLuint frameBuffer, Skybox* skybox)
{
	static std::vector<const char*> items ={ "Right face" ,"Left face","Top face","Bottom face","Front face","Back face" };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	ImGui::Begin("Test");
	ImGui::Text("Model view settings");
	ImGui::InputFloat3("Position", &position[0]);
	ImGui::InputFloat3("Rotation", &rotation[0]);
	ImGui::InputFloat3("Scale", &scale[0]);
	ImGui::Separator();
	ImGui::Text("Model rendering settings.");
	ImGui::Checkbox("Wireframe?", &m_pRendererSettings->m_isWireFrameRenderingOn);
	ImGui::Checkbox("Albedo?", &m_pRendererSettings->m_isRenderAlbedoTextureOn);
	ImGui::Checkbox("Specular?", &m_pRendererSettings->m_isRenderSpecularTextureOn);
	ImGui::Checkbox("Normals?", &m_pRendererSettings->m_isRenderNormalTextureOn);
	ImGui::Checkbox("Ambient occlusion?", &m_pRendererSettings->m_isRenderAmbientOcclusionTextureOn);
	ImGui::End();
	//ModelData modelData = (*m_pCurrentlyLoadedModel)->GetModelData();
	ImGui::Begin("Model data.");
	ImGui::Text("Object triangle count:%d", 1);
	ImGui::Text("Object vertex count:%d",2);
	ImGui::Text("Object face count:%d", 3);
	ImGui::Text("Texture count:%d", 6);
	ImGui::Text("File path %s", "Dummy path");

	ImGui::Separator();
	ImGui::Text("Scene settings.");
	ImGui::Checkbox("UseWorldGrid?", &m_pRendererSettings->m_isWireGridOn);
	ImGui::Checkbox("Enable lights?", &m_pRendererSettings->m_isRenderingLightOn);
	if (m_pRendererSettings->m_isRenderingLightOn)
	{
		static float color;
		static int lightCount;
		ImGui::InputInt("Light count", &lightCount);
		ImGui::ColorPicker4("Light color", &color);
	}
	ImGui::Checkbox("Enable skybox?", &m_pRendererSettings->m_isSkyboxOn);
	if (m_pRendererSettings->m_isSkyboxOn)
	{
		ImGui::Separator();
		if (ImGui::Button("Load skybox textures."))
		{
			onSkyboxLoadDialogCallback();
		}
		ImTextureID test = (ImTextureID)frameBuffer;
		if(skybox != nullptr)
			test = (ImTextureID)skybox->m_faceTextures[0]->GetTextureHandle();
		ImGui::Image((ImTextureID)test, {50,50}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		DrawComboBox("Top face", items);

		if (skybox != nullptr)
			test = (ImTextureID)skybox->m_faceTextures[1]->GetTextureHandle();
		ImGui::Image((ImTextureID)test, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		DrawComboBox("Bottom face", items);

		if (skybox != nullptr)
			test = (ImTextureID)skybox->m_faceTextures[2]->GetTextureHandle();

		ImGui::Image((ImTextureID)test, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		DrawComboBox("Right face", items);

		if (skybox != nullptr)
			test = (ImTextureID)skybox->m_faceTextures[3]->GetTextureHandle();
		ImGui::Image((ImTextureID)test, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		DrawComboBox("Left face", items);

		if (skybox != nullptr)
			test = (ImTextureID)skybox->m_faceTextures[4]->GetTextureHandle();
		ImGui::Image((ImTextureID)test, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		DrawComboBox("Front face", items);

		if (skybox != nullptr)
			test = (ImTextureID)skybox->m_faceTextures[5]->GetTextureHandle();
		ImGui::Image((ImTextureID)test, { 50,50 }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::SameLine();
		DrawComboBox("Back face", items);

		//TODO:Add texture preview;
		ImGui::Separator();
	}
	ImGui::End();
	ImGui::Begin("Loading panel");
	ImGui::Text("Loading stuff here.");
	ImGui::End();

	ImGui::Begin("Loading panel");
	if (ImGui::Button("Open obj file"))
	{
		if (onModelLoadDialogCallback != NULL)
			onModelLoadDialogCallback();
	}

	ImGui::Text("Loading stuff here.");
	ImGui::End();

	ImGui::Begin("Scene");
	{
		m_sceneViewImgSize = ImGui::GetWindowSize();
		ImGui::BeginChild("GameRender");
		ImGui::Image((ImTextureID)frameBuffer, m_sceneViewImgSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::EndChild();
	};
	ImGui::End();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OBJ_Viewer::UIRenderer::DrawComboBox(const char* initialValue,const std::vector<const char*>& items)
{
	const char* current_item = NULL;

	if (ImGui::BeginCombo(" ", initialValue)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < items.size(); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
			/*	if (skybox != nullptr)
					skybox->SwapSkyboxFaceTextures(SKYBOX_FACE_LEFT, static_cast<SkyboxFace>(n));*/
			}
		}
		ImGui::EndCombo();
	}
}
