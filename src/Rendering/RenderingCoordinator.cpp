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
	GLFWwindow* window = this->m_appState->GetGlobalAppWindow().GetGLFW_Window();
	glEnable(GL_DEPTH_TEST);
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
	Renderer::IsWireFrameOn(appSettings.m_isWireFrameRenderingOn);

	framebuffer.BindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	for (const auto& mesh : Model.GetModelMeshes())
	{
		m_rendererShaders.colorShader.UseShader();
		if(appSettings.m_isRenderAlbedoTextureOn && mesh->GetMaterial()->m_albedoTexture.get() != nullptr)
			Renderer::BindMaterialTexture(m_rendererShaders.colorShader, mesh->GetMaterial()->m_albedoTexture, GL_TEXTURE1, "material.albedoTexture");

		m_rendererShaders.colorShader.UniformSet3FloatVector("material.color", mesh->GetMaterial()->color);
		
		Renderer::RenderMesh(m_rendererShaders.colorShader, *mesh, *m_Camera);
	}
	if (skybox != nullptr && appSettings.m_isSkyboxOn)
		Renderer::RenderSkybox(m_rendererShaders.skyboxShader, *skybox, *m_Camera);
	framebuffer.UnbindFramebuffer();
}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(AppState* appState)
	:m_UILayer(appState,ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None)
{
	m_appState = appState;
	auto WinMetrics = appState->GetGlobalAppWindow().GetWindowSize();
	m_Camera =std::make_unique<Camera>(5.0f, WinMetrics.m_winWidth, WinMetrics.m_winHeight,
		m_appState->GetGlobalInputHandler());
	appState->GetGlobalAppWindow().GetMousePosNotifier().Attach(m_Camera.get());
	appState->GetGlobalAppWindow().GetScrollChangeNotifier().Attach(m_Camera.get());
	appState->GetGlobalAppWindow().GetWindowSizeChangeNotifier().Attach(m_Camera.get());
}





