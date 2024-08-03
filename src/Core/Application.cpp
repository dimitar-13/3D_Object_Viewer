#include "pch.h"
#include "Application.h"
#include"Rendering/RenderingCoordinator.h"


OBJ_Viewer::Application::Application(Window& appWindow):
	m_window(appWindow),
	m_sceneViewport(Viewport{ 0,0, appWindow.GetWindowSize().width, appWindow.GetWindowSize().height})
{
	m_window.SetOnEventCallback(std::bind(&Application::OnEvent,this, std::placeholders::_1));

	glViewport(0, 0, appWindow.GetWindowSize().width, appWindow.GetWindowSize().height);
	InitImGui();

	m_inputHandler = std::make_shared<InputHandler>();
	AddEventListener(m_inputHandler);
	m_appRenderingCoordinator = std::make_shared<RenderingCoordinator>(*this);
	AddEventListener(m_appRenderingCoordinator);

	m_appInitStatusSuccsess = true;
}

void OBJ_Viewer::Application::AppStartRenderLoop()
{
	m_appRenderingCoordinator->RenderLoop();
}

void OBJ_Viewer::Application::SubmitSceneViewportSize(const Viewport& newViewport)
{
	if (m_sceneViewport.GetViewport() == newViewport)
		return;

	m_sceneViewport.UpdateSceneViewport(newViewport);

	glViewport(0, 0, newViewport.width, newViewport.height);

	SceneViewportResizeEvent e(m_sceneViewport.GetViewport());
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

	ImGui_ImplGlfw_InitForOpenGL(this->m_window.GetGLFW_Window(), true);
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
	if (winEvent.GetEventType() == EventType_kKeyPressed )
	{
		KeyboardKeyEvent e = dynamic_cast<KeyboardKeyEvent&>(winEvent);
		OnAppKeyBindPressed(e);
	}
	else if (winEvent.GetEventType() == EventType_kWindowSizeChanged && m_isUIHidden)
	{
		WindowResizeEvent e = dynamic_cast<WindowResizeEvent&>(winEvent);
		Size2D win_size = e.GetWindowSize();

		static Viewport viewport{};
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = win_size.width;
		viewport.height = win_size.height;
		SubmitSceneViewportSize(viewport);
	}
}

void OBJ_Viewer::Application::OnAppKeyBindPressed(KeyboardKeyEvent& e)
{
	m_isUIHidden = e.GetKeyCode() == AppKeyBinding_kHideUI && e.GetKeyAction() == GLFW_PRESS ? !m_isUIHidden : m_isUIHidden;
	if (m_isUIHidden && e.GetKeyCode() == AppKeyBinding_kHideUI)
	{
		static Viewport viewport{};
		/* If we do 'static Size2D winSize = m_window->GetWindowSize();' the winSize wont update its values because this is a
		 * copy constructor initialization meaning since this variable is marked as static will be done only once and each other time it wont
		 * update its value.*/
		static Size2D win_size = {};
		win_size = m_window.GetWindowSize();
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = win_size.width;
		viewport.height = win_size.height;
		SubmitSceneViewportSize(viewport);

		m_inputHandler->SetCurrentlyFocusedWindow(APP_FOCUS_REGIONS::kUI_SceneWindowName);
	}
}
