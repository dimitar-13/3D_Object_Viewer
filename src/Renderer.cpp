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
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
		RenderScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		RenderImGui();

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
	m_mainRenderer.RenderObject(m_rendererShaders.colorShader, *m_currentlyLoadedModel, *m_Camera);
}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Window* windowHandler)/*:m_Camera(})*/
{
	m_windowHandler = windowHandler;
	m_Camera =std::make_unique<Camera>(5.0f,m_windowHandler->GetWindowSize().m_winWidth,m_windowHandler->GetWindowSize().m_winHeight);
	m_windowHandler->GetMousePosNotifier().Attach(m_Camera.get());
	m_windowHandler->GetScrollChangeNotifier().Attach(m_Camera.get());
	m_windowHandler->GetWindowSizeChangeNotifier().Attach(m_Camera.get());
	m_currentlyLoadedModel.reset(GenerateCubeModel());

	CreateFrameBuffer();

}

void OBJ_Viewer::RenderingCoordinator::RenderImGui()
{
	//Testing values
	float xPos = 1.0f;
	glm::vec3 position = { 0,0,0 };
	glm::vec3 scale = { 1,1,1 };
	glm::vec3 rotation = { 0,0,0 };

	uint32_t vertexCount = 4050, triangleCount = 2323, faceCount = 23232;
	//TODO:Extract code to differend imgui manager;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	bool p_open = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration;
	if (opt_fullscreen)
	{
		//This should not be here;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	//Right panel for model and rendering settings.
	ImGui::Begin("Test");
	ImGui::Text("Model view settings");
	ImGui::InputFloat3("Position", &position[0]);
	ImGui::InputFloat3("Rotation", &rotation[0]);
	ImGui::InputFloat3("Scale", &scale[0]);
	ImGui::Separator();
	ImGui::Text("Model rendering settings.");
	ImGui::Checkbox("Wireframe?", &m_rendererSettings.m_isWireFrameRenderingOn);
	ImGui::Checkbox("Albedo?", &m_rendererSettings.m_isRenderAlbedoTextureOn);
	ImGui::Checkbox("Specular?", &m_rendererSettings.m_isRenderSpecularTextureOn);
	ImGui::Checkbox("Normals?", &m_rendererSettings.m_isRenderNormalTextureOn);
	ImGui::Checkbox("Ambient occlusion?", &m_rendererSettings.m_isRenderAmbientOcclusionTextureOn);
	ImGui::End();

	ImGui::Begin("Model data.");
	ImGui::Text("Object triangle count:%d", triangleCount);
	ImGui::Text("Object vertex count:%d", vertexCount);
	ImGui::Text("Object face count:%d", faceCount);
	ImGui::Text("Texture count:%d", 6);
	ImGui::Text("File path %s", "Dummy path");

	ImGui::Separator();
	ImGui::Text("Scene settings.");
	ImGui::Checkbox("UseWorldGrid?", &m_rendererSettings.m_isWireGridOn);
	ImGui::Checkbox("Enable lights?", &m_rendererSettings.m_isRenderingLightOn);
	ImGui::Checkbox("Enable skybox?", &m_rendererSettings.m_isSkyboxOn);
	ImGui::End();
	ImGui::Begin("Loading panel");
	ImGui::Text("Loading stuff here.");
	ImGui::End();

	ImGui::Begin("Loading panel");
	if (ImGui::Button("Open obj file"))
	{
		DialogWrapper dialog;
		if (dialog.GetDialogPath() != NULL)
			this->m_currentlyLoadedModel.reset(ModelLoader::LoadModel(dialog.GetDialogPath()));
	}

	ImGui::Text("Loading stuff here.");
	ImGui::End();

	ImGui::Begin("Scene");
	{
		// Using a Child allow to fill all the space of the window.
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 wsize = ImGui::GetWindowSize();
		
		ImGui::BeginChild("GameRender");
		ImGui::Image((ImTextureID)this->m_framebufferTextureID, wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	};
	ImGui::End();

	ImGui::End();



	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OBJ_Viewer::RenderingCoordinator::CreateFrameBuffer()
{
	glGenFramebuffers(1, &this->m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer);
	
	glGenTextures(1, &this->m_framebufferTextureID);
	glBindTexture(GL_TEXTURE_2D, this->m_framebufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1000, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_framebufferTextureID, 0);
	glGenRenderbuffers(1, &this->m_readBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_readBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1000, 1000);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_readBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR]:Failed to create valid framebuffer." << '\n';
		return;
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
