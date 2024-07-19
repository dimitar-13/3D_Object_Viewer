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
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window))
	{
		if (m_currentWindowState != WINDOW_STATE_MINIMIZED) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!m_application.isUIHidden())
				m_UILayer->RenderUI();

			RenderScene();
			glfwSwapBuffers(window);
		}
		glfwPollEvents();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	auto& appSettings =	m_application.GetScene_RefSettings();
	auto& framebuffer =	m_application.GetSceneFrameBuffer();

	//Set up the renderer based on the settings;
 
	//framebuffer.BindFramebuffer();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//I didnt thought of this but if you want to do blending and you render the grid last do yourself a favor and set the alpha here to 0;
	glClearColor(0, 0, 0, 0);

	m_sceneRenderer->RenderScene(appSettings);

	//framebuffer.UnbindFramebuffer();



	if (m_application.isUIHidden())
	{
		m_sceneRenderer->RenderFramebufferSampledFullScreenQuad();
	}
}

void OBJ_Viewer::RenderingCoordinator::OnEvent(Event& e)
{
	if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_WINDOW_STATE_CHANGED)
		m_currentWindowState = dynamic_cast<WindowStateChangedEvent&>(e).GetWindowState();
}


OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Application& application):m_application(application)
{
	std::shared_ptr<RenderingMediator> mediator = std::make_shared<RenderingMediator>();
	m_sceneRenderer = std::make_shared<SceneRenderer>(application, mediator);
	m_application.AddEventListener(m_sceneRenderer);
	m_UILayer = std::make_unique<UILayer>(m_application, mediator, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration, ImGuiDockNodeFlags_None);

}





