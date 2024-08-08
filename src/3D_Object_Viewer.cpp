#include "pch.h"
#include "Core/Application.h"
#include "Controls/InputHandler.h"
#include "Rendering/RenderingCoordinator.h"
#include "Logging/App_Logger.h"
#include "Core/Config.h"



static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);

enum ExitStatus_
{
	ExitStatus_kOnApplicationCloses = 1,
	ExitStatus_kGLFW_InitFailed = -1,
	ExitStatus_kGLEW_InitFailed = -2,
	ExitStatus_kWindowContextCreationFailed = -3,
	ExitStatus_kApplicationCreationFailled = -4,
	ExitStatus_ApplicationExitSuccsess = ExitStatus_kOnApplicationCloses
};

int main()
{
#pragma region Const startup app data

	constexpr int kStartupWindowWidth = 1200;
	constexpr int kStartupWindowHeight = 1500;
	const char* winTitle = "3D_viewer";
	constexpr OBJ_Viewer::Size2D kWindowStartupSize = { kStartupWindowWidth,kStartupWindowHeight };
  
    const std::string kImGUISaveFilePath = OBJ_Viewer::ProjectPathHelper::GetCurrentExecutableFilePath().append("project/imgui.ini");
#pragma endregion

#pragma region Application dependencies and Window inilizing 

	OBJ_Viewer::Logger::init();
	if (glfwInit() == GLFW_FALSE)
	{
		LOGGER_LOG_FATAL("GLFW failed to initialize");

		return ExitStatus_::ExitStatus_kGLFW_InitFailed;
	}

	OBJ_Viewer::Window appWindow(kWindowStartupSize, winTitle);

	if (!appWindow.isWinContextInitialized())
	{
		LOGGER_LOG_FATAL("GLFW failed to create a valid window context.");

		return ExitStatus_::ExitStatus_kWindowContextCreationFailed;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();

		LOGGER_LOG_FATAL("GLEW failed to initialize.");

		return ExitStatus_::ExitStatus_kGLEW_InitFailed;
	}

	glewExperimental = GL_TRUE;
    stbi_set_flip_vertically_on_load(true);

	stbi_flip_vertically_on_write(true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::LoadIniSettingsFromDisk(kImGUISaveFilePath.c_str());

    ImGui_ImplGlfw_InitForOpenGL(appWindow.GetGLFW_Window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");


#ifdef OBJ_VIEWER_LEVEL_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif // OBJ_VIEWER_LEVEL_DEBUG

#pragma endregion

#pragma region Application creation and error checking

	OBJ_Viewer::Application app(appWindow);

	if (!app.isAppInitStatusSuccess())
	{
		app.~Application();
		LOGGER_LOG_FATAL("App failed to launch successfully");

		return ExitStatus_::ExitStatus_kWindowContextCreationFailed;
	}
	LOGGER_LOG_INFO("App launched successfully");

#pragma endregion

	app.AppStartRenderLoop();

	LOGGER_LOG_INFO("App exited successfully.");

    ImGui::SaveIniSettingsToDisk(kImGUISaveFilePath.c_str());

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

	return ExitStatus_::ExitStatus_ApplicationExitSuccsess;
}
static void APIENTRY glDebugOutput(GLenum source, GLenum type,
	unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;


	constexpr size_t kStringMessageReserveSize = 50;

	std::string errorString;
	errorString.reserve(kStringMessageReserveSize);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				  errorString += "ERROR[API]:"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   errorString += "ERROR[Window System]:"; break;
		//case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "ERROR[Shader Compiler]:"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     errorString += "ERROR[Third Party]:"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     errorString += "ERROR[Application]:"; break;
	case GL_DEBUG_SOURCE_OTHER:           errorString += "ERROR[Other]:"; break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               errorString += "Type:[Error]";

		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: errorString += "Type:[Deprecated Behaviour]"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  errorString += "Type:[Undefined Behaviour]"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         errorString += "Type:[Performance]"; break;
	}

	LOGGER_LOG_ERROR("{0} message:{1}", errorString.c_str(), message);
}