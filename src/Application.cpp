#include "Application.h"

bool OBJ_Viewer::Application::init(int winWidth, int winHeight, const char* winTitle)
{
	this->m_windowMetrics = { winWidth ,winHeight };
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "[ERROR]:GLFW failed to initialize." << '\n';
		return false;
	}
	m_appWindow = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL);
	if (m_appWindow == NULL)
	{
		glfwTerminate();
		std::cout << "[ERROR]:GLFW failed to create window." << '\n';
		return false;
	}
	glfwMakeContextCurrent(m_appWindow);

	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(m_appWindow);
		glfwTerminate();
		std::cout << "[ERROR]:GLEW failed to initialize." << '\n';
		return false;
	}
	glewExperimental = GL_TRUE;
	InitImGui();
	return true;
}
OBJ_Viewer::Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(m_appWindow);
	glfwTerminate();
}
void OBJ_Viewer::Application::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(m_appWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}