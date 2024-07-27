#include "pch.h"
#include "AppProfiler.h"

double OBJ_Viewer::FPSLogger::s_previousTime{};
double OBJ_Viewer::FPSLogger::s_timeElapsed{};
size_t OBJ_Viewer::FPSLogger::s_FPS_current{};

void OBJ_Viewer::FPSLogger::log()
{
#ifdef OBJ_VIEWER_LEVEL_DEBUG
	s_timeElapsed += glfwGetTime() - s_previousTime;
	s_previousTime = glfwGetTime();
	s_FPS_current++;
	if (s_timeElapsed >= 1.)
	{
		LOGGER_LOG_INFO("FPS:{0}", s_FPS_current);
		LOGGER_LOG_INFO("Frame time :{0}", 1.0 / s_FPS_current);
		s_timeElapsed = 0.f;
		s_FPS_current = 0;
	}
#endif
}

