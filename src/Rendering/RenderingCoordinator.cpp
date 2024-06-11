#include "RenderingCoordinator.h"
#include"ShaderClass.h"
#include"Application.h"
#include"ModelLoader.h"
#include"MeshGeneratingMethods.h"
#include"DialogWrapper.h"
#include"imgui_internal.h"
#include"imgui.h"

std::vector <glm::vec3> planeVerts = {
	{ -1.0f,1.0f,0.0f},
	{-1.0f, -1.0f, 0.0},
	{1.0f,  1.0f, 0.0},
	{1.0f, -1.0f, 0.0}
};
std::vector<GLuint> planeIndexData =
{
	 0, 1, 2,
	 2, 1, 3
};

void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	GLFWwindow* window = this->m_appState->GetGlobalAppWindow().GetGLFW_Window();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);

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

	framebuffer.BindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//I didnt thought of this but if you want to do blending and you render the grid last do yourself a favor and set the alpha here to 0;
	glClearColor(0, 0, 0, 0);
	
	for (const auto& mesh : Model.GetModelMeshes())
	{

		Renderer::IsWireFrameOn(appSettings.m_isWireFrameRenderingOn);

		m_rendererShaders.colorShader.UseShader();
		if(appSettings.m_isRenderAlbedoTextureOn && mesh->GetMaterial()->m_albedoTexture.get() != nullptr)
			Renderer::BindMaterialTexture(m_rendererShaders.colorShader, mesh->GetMaterial()->m_albedoTexture, GL_TEXTURE1, "material.albedoTexture");

		m_rendererShaders.colorShader.UniformSet3FloatVector("material.color", mesh->GetMaterial()->color);
		if (appSettings.m_isRenderingLightOn)
		{
			Renderer::RenderMeshLight(m_rendererShaders.lightShader, *mesh, *m_Camera, appSettings.lightInfo);
			continue;
		}
		Renderer::RenderMesh(m_rendererShaders.colorShader, *mesh, *m_Camera);
	}

	if (skybox != nullptr && appSettings.m_isSkyboxOn)
	{
		Renderer::IsWireFrameOn(false);
		Renderer::RenderSkybox(m_rendererShaders.skyboxShader, *skybox, *m_Camera);
	}

	if (appSettings.m_isWireGridOn)
	{
		Renderer::IsWireFrameOn(false);
		m_rendererShaders.gridShader.UseShader();
		m_rendererShaders.gridShader.UniformSet3FloatVector("cameraPosition", m_Camera->GetCameraPos());
    	Renderer::RenderGrid(m_rendererShaders.gridShader, m_SceneGridVAO, *m_Camera, appSettings.m_gridData);
	}

	framebuffer.UnbindFramebuffer();
}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(AppState* appState)
	:m_UILayer(appState,ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None),
	m_SceneGridVAO(planeVerts, planeIndexData)
{
	m_appState = appState;
	auto WinMetrics = appState->GetGlobalAppWindow().GetWindowSize();
	m_Camera =std::make_unique<Camera>(5.0f, WinMetrics.m_winWidth, WinMetrics.m_winHeight,
		m_appState->GetGlobalInputHandler());
	appState->GetGlobalAppWindow().GetMousePosNotifier().Attach(m_Camera.get());
	appState->GetGlobalAppWindow().GetScrollChangeNotifier().Attach(m_Camera.get());
	appState->GetGlobalAppWindow().GetWindowSizeChangeNotifier().Attach(m_Camera.get());
}





