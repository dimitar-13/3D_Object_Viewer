#include "RenderingCoordinator.h"
#include"ShaderClass.h"
#include"Application.h"
#include"ModelLoader.h"
#include"MeshGeneratingMethods.h"
#include"DialogWrapper.h"
#include"imgui_internal.h"
#include"imgui.h"

std::vector <OBJ_Viewer::Vertex> planeVerts = {
	{glm::vec3(-1.0f,1.0f,0.0f),  glm::vec2(0)},
	{glm::vec3(-1.0f, -1.0f, 0.0), glm::vec2(0)},
	{glm::vec3(1.0f,  1.0f, 0.0),  glm::vec2(0)},
	{glm::vec3(1.0f, -1.0f, 0.0),  glm::vec2(0)} 
};
std::vector<GLuint> planeIndexData =
{
	 0, 1, 2,
	 2, 3, 1
};

void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	GLFWwindow* window = this->m_appState->GetGlobalAppWindow().GetGLFW_Window();
	glEnable(GL_DEPTH_TEST |  GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_UILayer.RenderUI();
		RenderScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	auto appSettings = *m_appState->GetScene_pSettings();
	auto& framebuffer = m_appState->GetSceneFrameBuffer();
	auto& Model = m_appState->GetSceneModel();
	const Skybox* skybox = m_appState->GetSceneSkybox();
	//Set up the renderer based on the settings;

	/*if(this->m_rendererSettings.m_isWireGridOn)
		this->m_mainRenderer.RenderObject();*/

	framebuffer.BindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	if (appSettings.m_isWireGridOn)
	{
		Renderer::IsWireFrameOn(false);
		m_rendererShaders.gridShader.UseShader();
		m_rendererShaders.gridShader.UniformSet3FloatVector("cameraPosition", m_Camera->GetCameraPos());
		Renderer::RenderGrid(m_rendererShaders.gridShader, m_SceneGrid, *m_Camera,appSettings.m_gridData);
	}
	if (skybox != nullptr && appSettings.m_isSkyboxOn)
	{
		Renderer::IsWireFrameOn(false);
		Renderer::RenderSkybox(m_rendererShaders.skyboxShader, *skybox, *m_Camera);
	}
	for (const auto& mesh : Model.GetModelMeshes())
	{
		Renderer::IsWireFrameOn(appSettings.m_isWireFrameRenderingOn);

		m_rendererShaders.colorShader.UseShader();
		if(appSettings.m_isRenderAlbedoTextureOn && mesh->GetMaterial()->m_albedoTexture.get() != nullptr)
			Renderer::BindMaterialTexture(m_rendererShaders.colorShader, mesh->GetMaterial()->m_albedoTexture, GL_TEXTURE1, "material.albedoTexture");

		m_rendererShaders.colorShader.UniformSet3FloatVector("material.color", mesh->GetMaterial()->color);
		
		Renderer::RenderMesh(m_rendererShaders.colorShader, *mesh, *m_Camera);
	}

	framebuffer.UnbindFramebuffer();
}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(AppState* appState)
	:m_UILayer(appState,ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None),
	m_SceneGrid(planeVerts, planeIndexData,glm::mat4(1))
{
	m_appState = appState;
	auto WinMetrics = appState->GetGlobalAppWindow().GetWindowSize();
	m_Camera =std::make_unique<Camera>(5.0f, WinMetrics.m_winWidth, WinMetrics.m_winHeight,
		m_appState->GetGlobalInputHandler());
	appState->GetGlobalAppWindow().GetMousePosNotifier().Attach(m_Camera.get());
	appState->GetGlobalAppWindow().GetScrollChangeNotifier().Attach(m_Camera.get());
	appState->GetGlobalAppWindow().GetWindowSizeChangeNotifier().Attach(m_Camera.get());
}





