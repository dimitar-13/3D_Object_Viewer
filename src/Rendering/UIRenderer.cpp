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
void OBJ_Viewer::UIRenderer::RenderUI(GLuint frameBuffer)
{
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
		ImGui::Button("Load front texture.");
		ImGui::Button("Load back texture.");
		ImGui::Button("Load top texture.");
		ImGui::Button("Load bottom texture.");
		ImGui::Button("Load left texture.");
		ImGui::Button("Load right texture.");
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
