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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	
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

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(Application::glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	glViewport(0, 0, metrics.width, metrics.height);
	InitImGui();

	m_inputHandler = std::shared_ptr<InputHandler>(new InputHandler());
	AddEventListener(m_inputHandler);
	m_sceneFramebuffer = std::make_unique<Framebuffer>(Size2D{STARTUP_WINDOW_WIDTH,STARTUP_WINDOW_HEIGHT}, FRAMEBUFFER_COLOR_ATTACHMENT);
	m_appRenderingCoordinator = std::make_unique<RenderingCoordinator>(*this);
	m_appRenderingCoordinator->RenderLoop();
}

void OBJ_Viewer::Application::glDebugOutput(GLenum source, GLenum type, 
	unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             std::cout << "ERROR[API]:"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "ERROR[Window System]:"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "ERROR[Shader Compiler]:"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "ERROR[Third Party]:"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "ERROR[Application]:"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "ERROR[Other]:"; break;
	}
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	}
	std::cout << "Message:" << message << '\n';
	/*switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}*/
}

void OBJ_Viewer::Application::ResizeBuffer(int newWidth, int newHeight)
{
	Size2D size = m_sceneFramebuffer->GetFramebufferSize();
	
	if ((size.width == newWidth && size.height == newHeight))
		return;
	glViewport(0, 0, newWidth, newHeight);

	this->m_sceneFramebuffer->ResizeFramebuffer({ newWidth,newHeight});

	FramebufferResizeEvent e(Size2D{ newWidth, newHeight });

	OnEvent(e);

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
	for (uint32_t i = 0; i < m_eventListeners.size(); i++)
	{
		if (std::shared_ptr<Listener> listener = m_eventListeners[i].lock())
		{
			listener->OnEvent(winEvent);
		}
	}
	if (winEvent.GetEventType() == EVENT_KEY_PRESSES )
	{
		KeyboardKeyEvent e = dynamic_cast<KeyboardKeyEvent&>(winEvent);
		OnAppKeyBindPressed(e);
	}
	else if (winEvent.GetEventType() == EVENT_WINDOW_SIZE_CHANGED && m_isUIHidden)
	{
		WindowResizeEvent e = dynamic_cast<WindowResizeEvent&>(winEvent);
		Size2D winSize = e.GetWindowSize();

		static SceneViewport viewport{};
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = winSize.width;
		viewport.height = winSize.height;
		UpdateSceneViewport(viewport);
	}
}

void OBJ_Viewer::Application::OnAppKeyBindPressed(KeyboardKeyEvent& e)
{
	m_isUIHidden = e.GetKeyCode() == KEY_BIND_HIDE_UI && e.GetKeyAction() == GLFW_PRESS ? !m_isUIHidden : m_isUIHidden;
	if (m_isUIHidden && e.GetKeyCode() == KEY_BIND_HIDE_UI)
	{
		static SceneViewport viewport{};
		/* If we do 'static Size2D winSize = m_window->GetWindowSize();' the winSize wont update its values because this is a
		 * copy constructor initialization meaning since this variable is marked as static will be done only once and each other time it wont
		 * update its value.*/
		static Size2D winSize = {};
		winSize = m_window->GetWindowSize();
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = winSize.width;
		viewport.height = winSize.height;
		UpdateSceneViewport(viewport);

		m_inputHandler->SetCurrentlyFocusedWindow(UI_LAYER_SCENE_WINDOW_NAME);
	}
}
