#include "Application.h"
#include"imgui_internal.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<iostream>
#include"Rendering/RenderingCoordinator.h"
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
	this->m_window = new Window(metrics, winTitle,std::bind(&Application::OnEvent,this,std::placeholders::_1));
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

	m_eventListeners.push_back(&m_inputHandler);
	m_sceneFramebuffer = new Framebuffer(STARTUP_WINDOW_WIDTH, STARTUP_WINDOW_HEIGHT, FRAMEBUFFER_COLOR_ATTACHMENT);
	m_appRenderingCoordinator = new RenderingCoordinator(*this);
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
	delete(m_sceneFramebuffer);
	delete(m_appRenderingCoordinator);
	glfwDestroyWindow(this->m_window->GetGLFW_Window());
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
	for (auto* listener : m_eventListeners)
	{
		listener->OnEvent(winEvent);
	}
}
