#include "RenderingCoordinator.h"
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

	/*if(this->m_rendererSettings.m_isWireGridOn)
		this->m_mainRenderer.RenderObject();*/
	Renderer::IsWireFrameOn(m_rendererSettings.m_isWireFrameRenderingOn);

	this->m_sceneFramebuffer.BindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	for (const auto& mesh : m_currentlyLoadedModel->GetModelMeshes())
	{
		m_rendererShaders.colorShader.UseShader();
		if(m_rendererSettings.m_isRenderAlbedoTextureOn && mesh->GetMaterial()->m_albedoTexture.get() != nullptr)
			Renderer::BindMaterialTexture(m_rendererShaders.colorShader, mesh->GetMaterial()->m_albedoTexture, GL_TEXTURE1, "material.albedoTexture");

		m_rendererShaders.colorShader.UniformSet3FloatVector("material.color", mesh->GetMaterial()->color);
		
		Renderer::RenderMesh(m_rendererShaders.colorShader, *mesh, *m_Camera);
	}
	if (m_Skybox.get() != nullptr && m_rendererSettings.m_isSkyboxOn)
		Renderer::RenderSkybox(m_rendererShaders.skyboxShader, *m_Skybox, *m_Camera);
	this->m_sceneFramebuffer.UnbindFramebuffer();
}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Window* windowHandler, InputHandler* pInputHandler):m_currentlyLoadedModel(GenerateCubeModel()),
	m_imGuiUIRenderer(ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None,
		&this->m_rendererSettings), m_sceneFramebuffer(1000,1000,FRAMEBUFFER_COLOR_ATTACHMENT)
{
	m_windowHandler = windowHandler;
	m_Camera =std::make_unique<Camera>(5.0f,m_windowHandler->GetWindowSize().m_winWidth,m_windowHandler->GetWindowSize().m_winHeight, pInputHandler);
	m_windowHandler->GetMousePosNotifier().Attach(m_Camera.get());
	m_windowHandler->GetScrollChangeNotifier().Attach(m_Camera.get());
	m_windowHandler->GetWindowSizeChangeNotifier().Attach(m_Camera.get());
	m_pInputHandler = pInputHandler;
	m_imGuiUIRenderer.SetOnModelLoadDialogCallback([this]() {
		this->LoadModel(); });
	std::array<const char*,6> skyboxFacePaths = {
		"C:/Users/MSI Katana 15/Desktop/skybox/skybox/right.png",
		"C:/Users/MSI Katana 15/Desktop/skybox/skybox/left.png",
		"C:/Users/MSI Katana 15/Desktop/skybox/skybox/bottom.png",
		"C:/Users/MSI Katana 15/Desktop/skybox/skybox/top.png",
		"C:/Users/MSI Katana 15/Desktop/skybox/skybox/front.png",
		"C:/Users/MSI Katana 15/Desktop/skybox/skybox/back.png"
	};
	this->m_Skybox = std::make_unique<Skybox>(skyboxFacePaths);
}

void OBJ_Viewer::RenderingCoordinator::RenderImGui()
{
	m_imGuiUIRenderer.RenderUI(this->m_sceneFramebuffer.GetFramebufferTexture()->GetTextureHandle());
}

void OBJ_Viewer::RenderingCoordinator::LoadModel()
{
	DialogWrapper dialog;
	if (dialog.GetDialogPath() != NULL)
	{
		ModelLoader loader;
		m_currentlyLoadedModel.reset(loader.LoadModel(dialog.GetDialogPath()));
	}
}



