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
			resize_buffer(winSize);
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
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	m_mainRenderer.RenderObject(m_rendererShaders.colorShader, *m_currentlyLoadedModel, *m_Camera);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Window* windowHandler):m_currentlyLoadedModel(GenerateCubeModel()),
	m_imGuiUIRenderer(ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None,
		&this->m_rendererSettings, m_currentlyLoadedModel.get())
{
	m_windowHandler = windowHandler;
	m_Camera =std::make_unique<Camera>(5.0f,m_windowHandler->GetWindowSize().m_winWidth,m_windowHandler->GetWindowSize().m_winHeight);
	m_windowHandler->GetMousePosNotifier().Attach(m_Camera.get());
	m_windowHandler->GetScrollChangeNotifier().Attach(m_Camera.get());
	m_windowHandler->GetWindowSizeChangeNotifier().Attach(m_Camera.get());
	CreateFrameBuffer();
}

OBJ_Viewer::RenderingCoordinator::~RenderingCoordinator()
{
	glDeleteTextures(1, &this->m_framebufferTextureID);
	glDeleteFramebuffers(1, &this->m_framebuffer);
	glDeleteRenderbuffers(1, &this->m_readBuffer);
}

void OBJ_Viewer::RenderingCoordinator::RenderImGui()
{
	m_imGuiUIRenderer.RenderUI(this->m_framebuffer);	
}
void OBJ_Viewer::RenderingCoordinator::resize_buffer(ImVec2 newSize)
{
	glBindTexture(GL_TEXTURE_2D, this->m_framebufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newSize.x, newSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_framebufferTextureID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, this->m_readBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newSize.x, newSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_readBuffer);

}
void OBJ_Viewer::RenderingCoordinator::CreateFrameBuffer()
{
	glGenFramebuffers(1, &this->m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer);

	glGenTextures(1, &this->m_framebufferTextureID);
	glBindTexture(GL_TEXTURE_2D, this->m_framebufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_windowHandler->GetWindowSize().m_winWidth, this->m_windowHandler->GetWindowSize().m_winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_framebufferTextureID, 0);
	glGenRenderbuffers(1, &this->m_readBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_readBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->m_windowHandler->GetWindowSize().m_winWidth, this->m_windowHandler->GetWindowSize().m_winHeight);
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

OBJ_Viewer::UIRenderer::UIRenderer(ImGuiWindowFlags imGuiWindowFlags, 
	ImGuiDockNodeFlags imGuiDockSpaceFlags, RendererSettings* pRendererSettings, Model* pCurrentlyLoadedModel)
{
	this->m_pCurrentlyLoadedModel = pCurrentlyLoadedModel;
	this->m_pRendererSettings = pRendererSettings;

	bool opt_fullScreen = true;
	bool opt_padding = false;
	bool p_open = true;
	this->m_imgGuiDockSpaceFlags = imGuiDockSpaceFlags;
	this->m_imGuiWindowFlags = imGuiWindowFlags;

	if (opt_fullScreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		this->m_imGuiWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		this->m_imgGuiDockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}
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
	bool open = true;
	uint32_t vertexCount = 4050, triangleCount = 2323, faceCount = 23232;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);


	ImGui::Begin("DockSpace Demo", &open, m_imGuiWindowFlags);
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
