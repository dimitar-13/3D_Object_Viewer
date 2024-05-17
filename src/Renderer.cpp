#include "Renderer.h"
#include "VertexAttributeObject.h"
#include"ShaderClass.h"
#include"Application.h"
#include"ModelLoader.h"
#include"MeshGeneratingMethods.h"
#include"DialogWrapper.h"
#include"imgui_internal.h"
#include"imgui.h"
void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	GLFWwindow* window = this->m_windowHandler->GetGLFW_Window();
	glEnable(GL_DEPTH_TEST);
	ImVec2 winSize = m_imGuiUIRenderer.GetSceneViewImgSize();
	while (!glfwWindowShouldClose(window))
	{
		RenderImGui();
		//Window resize event don't work since pollEvents happens before ImGui rendering(from where we get the size of our window).
		if (winSize.x != m_imGuiUIRenderer.GetSceneViewImgSize().x && winSize.y != m_imGuiUIRenderer.GetSceneViewImgSize().y)
		{
			winSize = m_imGuiUIRenderer.GetSceneViewImgSize();
			glViewport(0, 0, winSize.x, winSize.y);
			this->m_sceneFramebuffer.ResizeFramebuffer(winSize.x, winSize.y);
		}
		RenderScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	//Set up the renderer based on the settings;

	/*if (this->m_rendererSettings.m_isSkyboxOn)
		this->m_mainRenderer.RenderObject(this->m_rendererShaders.skyboxShader);
	if(this->m_rendererSettings.m_isWireGridOn)
		this->m_mainRenderer.RenderObject();*/
	if (m_rendererSettings.m_isWireFrameRenderingOn)
		m_mainRenderer.EnableWireFrame();
	if (!m_rendererSettings.m_isWireFrameRenderingOn)
		m_mainRenderer.DisableWireFrame();
	//Submit to render;
	//m_mainRenderer.RenderObject(/*Shader to use*/, *m_currentlyLoadedModel);
	this->m_sceneFramebuffer.BindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	m_mainRenderer.RenderObject(m_rendererShaders.colorShader, *m_currentlyLoadedModel, *m_Camera);
	this->m_sceneFramebuffer.UnbindFramebuffer();

}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Window* windowHandler):m_currentlyLoadedModel(GenerateCubeModel()),
	m_imGuiUIRenderer(ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None,
		&this->m_rendererSettings, m_currentlyLoadedModel.get()), m_sceneFramebuffer(1000,1000,FRAMEBUFFER_COLOR_ATTACHMENT)
{
	m_windowHandler = windowHandler;
	m_Camera =std::make_unique<Camera>(5.0f,m_windowHandler->GetWindowSize().m_winWidth,m_windowHandler->GetWindowSize().m_winHeight);
	m_windowHandler->GetMousePosNotifier().Attach(m_Camera.get());
	m_windowHandler->GetScrollChangeNotifier().Attach(m_Camera.get());
	m_windowHandler->GetWindowSizeChangeNotifier().Attach(m_Camera.get());
}

void OBJ_Viewer::RenderingCoordinator::RenderImGui()
{
	m_imGuiUIRenderer.RenderUI(this->m_sceneFramebuffer.GetFramebufferTextureHandle());
}

void OBJ_Viewer::Renderer::RenderObject(const ShaderClass& shaderToUse, const Model& modelToRender, const Camera& mainCamera)
{
	shaderToUse.UseShader();
	shaderToUse.UniformSet4x4FloatMatrix("ViewProjMatrix", mainCamera.GetViewProjMatrix());
	for (const auto& mesh : modelToRender.GetModelMeshes())
	{
		mesh.GetMeshVAO().BindBuffer();
		glDrawElements(GL_TRIANGLES, mesh.GetMeshVAO().GetIndexCount(), GL_UNSIGNED_INT, NULL);
		mesh.GetMeshVAO().UnBind();
	}
}

OBJ_Viewer::UIRenderer::UIRenderer(ImGuiWindowFlags imGuiWindowFlags, 
	ImGuiDockNodeFlags imGuiDockSpaceFlags, RendererSettings* pRendererSettings, Model* pCurrentlyLoadedModel)
{
	this->m_pCurrentlyLoadedModel = pCurrentlyLoadedModel;
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

	ImGui::Begin("Model data.");
	ImGui::Text("Object triangle count:%d", triangleCount);
	ImGui::Text("Object vertex count:%d", vertexCount);
	ImGui::Text("Object face count:%d", faceCount);
	ImGui::Text("Texture count:%d", 6);
	ImGui::Text("File path %s", "Dummy path");

	ImGui::Separator();
	ImGui::Text("Scene settings.");
	ImGui::Checkbox("UseWorldGrid?", &m_pRendererSettings->m_isWireGridOn);
	ImGui::Checkbox("Enable lights?", &m_pRendererSettings->m_isRenderingLightOn);
	ImGui::Checkbox("Enable skybox?", &m_pRendererSettings->m_isSkyboxOn);
	ImGui::End();
	ImGui::Begin("Loading panel");
	ImGui::Text("Loading stuff here.");
	ImGui::End();

	ImGui::Begin("Loading panel");
	if (ImGui::Button("Open obj file"))
	{
		DialogWrapper dialog;
		/*if (dialog.GetDialogPath() != NULL)
			model->reset(ModelLoader::LoadModel(dialog.GetDialogPath()));*/
	}

	ImGui::Text("Loading stuff here.");
	ImGui::End();

	ImGui::Begin("Scene");
	{
		m_sceneViewImgSize =ImGui::GetWindowSize();
		ImGui::BeginChild("GameRender");
		ImGui::Image((ImTextureID)frameBuffer, m_sceneViewImgSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::EndChild();
	};
	ImGui::End();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
