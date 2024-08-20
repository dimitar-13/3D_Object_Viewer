#pragma once
#include "Logging/App_Logger.h"
#include <chrono>
namespace OBJ_Viewer {

	class FPSLogger
	{
	public:
		static void log();
	private:
		static double s_previousTime;
		static double s_timeElapsed;
		static size_t s_FPS_previous;
		static size_t s_FPS_current;
	};

#ifdef OBJ_VIEWER_LEVEL_DEBUG
	class Timer
	{
	public:
		Timer()
		{
			m_startTime = std::chrono::high_resolution_clock::now();
		}
		~Timer()
		{
			Stop();
		}
		void Stop() const
		{
			TimerClock endPoint = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

			auto duration = end - start; 
			auto ms = duration * .001;
			LOGGER_LOG_INFO("Task begin time: {0}us \n Task end time : {1}us \n Task duration : {2}us {3}ms",
				end, end, duration, ms);
		}
	private:
		using TimerClock = std::chrono::time_point<std::chrono::high_resolution_clock>;
		TimerClock m_startTime;
	};
#else
    class Timer
    {

    };

#endif // OBJ_VIEWER_LEVEL_DEBUG
}

