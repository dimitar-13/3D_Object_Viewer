#include "Application.h"
#include"imgui_internal.h"
OBJ_Viewer::Application::Application()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "[ERROR]:GLFW failed to initialize." << '\n';
	}
	
	WindowMetrics metrics = { 1200,1500 };
	const char* winTitle = "3D_viewer";
	this->m_window = new Window(metrics, winTitle);
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
	glViewport(0, 0, metrics.m_winWidth, metrics.m_winHeight);
	InitImGui();
}

OBJ_Viewer::Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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