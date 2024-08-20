#include "pch.h"
#include "App_Logger.h"

const char* APP_CONSOLE_NAME = "App console";

std::shared_ptr<spdlog::logger> OBJ_Viewer::Logger::s_appConsole;
void OBJ_Viewer::Logger::init()
{
	spdlog::set_pattern("[%T]:%^ %v %$ ");
	s_appConsole = spdlog::stdout_color_mt(APP_CONSOLE_NAME);
}
