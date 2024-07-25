#pragma once

#include "pch.h"

namespace OBJ_Viewer
{
	class Logger
	{
	public:
		static void init();
		static std::shared_ptr<spdlog::logger> GetAppConsole() { return s_appConsole; }
	private:
		static std::shared_ptr<spdlog::logger> s_appConsole;
	};
}
#ifdef OBJ_VIEWER_LEVEL_DEBUG

#define LOGGER_LOG_INFO(...)  OBJ_Viewer::Logger::GetAppConsole()->info(__VA_ARGS__)
#define LOGGER_LOG_TRACE(...)  OBJ_Viewer::Logger::GetAppConsole()->trace(__VA_ARGS__)
#define LOGGER_LOG_ERROR(...) OBJ_Viewer::Logger::GetAppConsole()->error(__VA_ARGS__)
#define LOGGER_LOG_FATAL(...) OBJ_Viewer::Logger::GetAppConsole()->critical(__VA_ARGS__)
#define LOGGER_LOG_WARN(...)  OBJ_Viewer::Logger::GetAppConsole()->warn(__VA_ARGS__)

#else
#define LOGGER_LOG_INFO(...)
#define LOGGER_LOG_TRACE(...)
#define LOGGER_LOG_ERROR(...)
#define LOGGER_LOG_FATAL(...)
#define LOGGER_LOG_WARN(...)

#endif // OBJ_VIEWER_LEVEL_DEBUG

