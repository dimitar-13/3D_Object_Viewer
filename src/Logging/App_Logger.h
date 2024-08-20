#pragma once

#include "pch.h"

namespace OBJ_Viewer
{
    /**
     * @brief A static logger class for managing log messages.
     *
     * This class utilizes the `spdlog` library (https://github.com/gabime/spdlog) to handle logging messages to the console.
     * It provides various logging levels such as info, trace, error, warn, and fatal, which can be used via the macros
     * `LOGGER_LOG_INFO`, `LOGGER_LOG_TRACE`, `LOGGER_LOG_ERROR`, `LOGGER_LOG_WARN`, and `LOGGER_LOG_FATAL`.
     *
     * Before using any of these macros, you must call the `init()` method to initialize the logger.
     *
     * @note In release builds, logging is disabled, and the macros effectively do nothing.
     *
     * Example usage:
     * ```cpp
     * Logger::init();
     *
     * LOGGER_LOG_INFO("The logger was initialized successfully yay.");
     * ```
     */
	class Logger
	{
	public:
        /**
         * @brief Initializes the logger system.
         *
         * Sets up the logging infrastructure by creating a console logger using the `spdlog` library.
         * Configures the console logger with a specific name and message format pattern.
         *
         * The message pattern used is `[%T]:%^ %v %$`, which translates to:
         * - `%T` : Timestamp of the log message.
         * - `%^` : Start of colored text (based on the log level).
         * - `%v` : The actual log message.
         * - `%$` : End of colored text.
         *
         * This setup ensures that log messages are displayed with timestamps and color-coded based on the message type.
         *
         * @note This method must be called before any logging operations to ensure that the logging system is properly initialized.
         */
		static void init();
        /**
         * @brief Retrieves the logger console instance.
         *
         * Provides access to the `spdlog` logger instance used for logging messages.
         * This method is primarily used by logging macros to obtain the logger instance
         * for outputting log messages.
         *
         * @returns A shared pointer to the `spdlog::logger` instance used by the application.
         *
         * @note This method is used internally by the logging macros and other parts of the logging system.
         */
		static std::shared_ptr<spdlog::logger> GetAppConsole() { return s_appConsole; }
	private:
		static std::shared_ptr<spdlog::logger> s_appConsole; ///< spdLog logging debug console.
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

