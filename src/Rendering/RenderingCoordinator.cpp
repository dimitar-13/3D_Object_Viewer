#include "RenderingCoordinator.h"
#include"imgui_internal.h"
#include"imgui.h"
#include <iostream>
#include <nfd.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"WindowHandler.h"
#include"Framebuffer.h"

void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	GLFWwindow* window = this->m_application.GetGlobalAppWindow().GetGLFW_Window();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_UILayer->RenderUI();
		RenderScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	auto& appSettings =	m_application.GetScene_RefSettings();
	auto& framebuffer =	m_application.GetSceneFrameBuffer();

	//Set up the renderer based on the settings;
	framebuffer.BindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//I didnt thought of this but if you want to do blending and you render the grid last do yourself a favor and set the alpha here to 0;
	glClearColor(0, 0, 0, 0);

	m_sceneRenderer->RenderScene(appSettings);

	framebuffer.UnbindFramebuffer();
}


OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Application& application):m_application(application)
{
	std::shared_ptr<RenderingMediator> mediator = std::make_shared<RenderingMediator>();
	m_sceneRenderer = std::make_shared<SceneRenderer>(application, mediator);
	m_UILayer = std::make_unique<UILayer>(m_application, mediator, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None);
}





