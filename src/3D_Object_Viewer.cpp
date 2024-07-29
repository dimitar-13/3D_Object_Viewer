#include "pch.h"
#include "Core/Application.h"
#include "Controls/InputHandler.h"
#include "Rendering/RenderingCoordinator.h"
#include "Logging/App_Logger.h"

static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);

enum ExitStatus
{
	EXIT_STATUS_ON_APP_CLOSED = 1,
	EXIT_STATUS_GLFW_INIT_FALIED = -1,
	EXIT_STATUS_GLEW_INIT_FAILED = -2,
	EXIT_STATUS_WINDOW_CONTEXT_CREATION_FAILED = -3,
	EXIT_STATUS_APP_CREATION_FAILED = -4,
	APP_EXIT_SUCCSESS = EXIT_STATUS_ON_APP_CLOSED
};


int main()
{
#pragma region Const startup app data


	constexpr int STARTUP_WINDOW_WIDTH = 1200;
	constexpr int STARTUP_WINDOW_HEIGHT = 1500;
	const char* winTitle = "3D_viewer";
	constexpr OBJ_Viewer::Size2D WINDOW_STARTUP_SIZE = { STARTUP_WINDOW_WIDTH,STARTUP_WINDOW_HEIGHT };
#pragma endregion

#pragma region Application dependencies and Window inilizing 

	OBJ_Viewer::Logger::init();
	if (glfwInit() == GLFW_FALSE)
	{
		LOGGER_LOG_FATAL("GLFW failed to initialize");

		return ExitStatus::EXIT_STATUS_GLFW_INIT_FALIED;
	}

	OBJ_Viewer::Window appWindow(WINDOW_STARTUP_SIZE, winTitle);

	if (!appWindow.isWinContextInitialized())
	{
		LOGGER_LOG_FATAL("GLFW failed to create a valid window context.");

		return ExitStatus::EXIT_STATUS_WINDOW_CONTEXT_CREATION_FAILED;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();

		LOGGER_LOG_FATAL("GLEW failed to initialize.");

		return ExitStatus::EXIT_STATUS_GLEW_INIT_FAILED;
	}

	glewExperimental = GL_TRUE;

	stbi_set_flip_vertically_on_load(true);
	stbi_flip_vertically_on_write(true);


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

		return ExitStatus::EXIT_STATUS_APP_CREATION_FAILED;
	}
	LOGGER_LOG_INFO("App launched successfully");

#pragma endregion

	app.AppStartRenderLoop();

	LOGGER_LOG_INFO("App exited successfully.");

	return ExitStatus::APP_EXIT_SUCCSESS;
}

static void APIENTRY glDebugOutput(GLenum source, GLenum type,
	unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;


	constexpr size_t STRING_MESSAGE_RESERVED_SIZE = 50;

	std::string errorString;
	errorString.reserve(STRING_MESSAGE_RESERVED_SIZE);

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