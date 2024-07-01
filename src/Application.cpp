#include "Application.h"
#include"imgui_internal.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<iostream>
#include"Rendering/RenderingCoordinator.h"
#include<iostream>
#define STARTUP_WINDOW_WIDTH 1200
#define STARTUP_WINDOW_HEIGHT 1500


OBJ_Viewer::Application::Application()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "[ERROR]:GLFW failed to initialize." << '\n';
	}
	
	Size2D metrics = { STARTUP_WINDOW_WIDTH,STARTUP_WINDOW_HEIGHT };
	const char* winTitle = "3D_viewer";
	this->m_window = std::make_unique <Window>(metrics, winTitle,std::bind(&Application::OnEvent,this,std::placeholders::_1));
	if (this->m_window->GetGLFW_Window() == NULL)
	{
		glfwTerminate();
		std::cout << "[ERROR]:GLFW failed to create window." << '\n';
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cout << "[ERROR]:GLEW failed to initialize." << '\n';
	}
	glewExperimental = GL_TRUE;
	glViewport(0, 0, metrics.width, metrics.height);
	InitImGui();

	m_inputHandler = std::unique_ptr<InputHandler>(new InputHandler());

	m_sceneFramebuffer = std::make_unique<Framebuffer>(STARTUP_WINDOW_WIDTH, STARTUP_WINDOW_HEIGHT, FRAMEBUFFER_COLOR_ATTACHMENT);
	m_appRenderingCoordinator = std::make_unique<RenderingCoordinator>(*this);
	m_appRenderingCoordinator->RenderLoop();
}

void OBJ_Viewer::Application::ResizeBuffer(int newWidth, int newHeight)
{
	Size2D size = m_sceneFramebuffer->GetFramebufferSize();
	if (!(size.width == newWidth && size.height == newHeight))
		glViewport(0, 0, newWidth, newHeight);

	this->m_sceneFramebuffer->ResizeFramebuffer(newWidth, newHeight);
}

OBJ_Viewer::Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}
void OBJ_Viewer::Application::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

	ImGui_ImplGlfw_InitForOpenGL(this->m_window->GetGLFW_Window(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void OBJ_Viewer::Application::OnEvent(Event& winEvent)
{
	m_inputHandler->OnEvent(winEvent);
	dynamic_cast<Listener&>(*m_appRenderingCoordinator).OnEvent(winEvent);
	for (uint32_t i = 0; i < m_eventListeners.size(); i++)
	{
		if (auto listener = m_eventListeners[i].lock())
		{
			listener->OnEvent(winEvent);
		}
	
	}
}
