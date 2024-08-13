#pragma once
#include "pch.h"
#include "WindowEvents.h"
#include "CommonAppData.h"
namespace OBJ_Viewer {

    /**
     * @brief Enum representing the physical window state.
     *
     * 
     * This enum is mainly used in the RenderingCoordinator and camera.
     * The main idea is that when window size is {0,0} we should not render anything and the camera should not change its size.
     * Because often a unminimize will be the previous size before the minimization.
     * 
     * - WindowState_kUnknown : For debugging.
     * - WindowState_kWindowMinimized: Window is minimized and now visible.
     * - WindowState_kWindowFullScreen: Window is full screen(This one is not beeing fully used currently).
     * - WindowState_kWindowStateNormal: Window is not minimized or full screen.
     * 
     * 
     */
	enum WindowState_
	{
		WindowState_kUnknown,          ///< For debugging.
		WindowState_kWindowMinimized,  ///< Window is minimized and now visible.
		WindowState_kWindowFullScreen, ///< Window is full screen.
		WindowState_kWindowStateNormal ///< Window is not minimized or full screen.
	};
    /**
     * @brief Window class for creating and managing a GLFW window.
     *
     * This class works with the https://github.com/glfw/glfw library to create and managed a window.
     * This class also handles the input that are received by the OS(inputs like scroll,mouse position, key press etc.) 
     * and submits the events to the application via event function.
     * The window also pools/saves its state see WindowState_ for more information.
     * 
     */
	class Window {
	public:
        /**
         * @brief Creates a GLFW window context.
         *
         * Creates a GLFW window context used the specified size and window tittle.
         * It logs error if there is any errors with the creation of the window.
         * Use 'IsWinContextInitialized()' too see if the window was initialize successfully.
         * @param windowMetrics Initial size of the window.
         * @param winTitle Title/Name of the window.
         * @note The class assumes that the glfw library was initialized successfully before hand.
         */
		Window(Size2D windowMetrics, const char* winTitle);
        /**
         * @brief Destroys the GLFW window using the 'glfwDestroyWindow()' function.
         */
		~Window() { glfwDestroyWindow(m_glfwWindow); }
        /**
         * @brief Retrieves the current window size.
         * 
         * @returns Returns the current size of the window.
         */
		Size2D GetWindowSize()const { return this->m_windowSize; }
        /**
         * @brief Returns the GLFW window handle.
         * 
         * This is used by the rendering loop and ImGUI.
         *
         * @returns Returns the GLFW window handle/pointer.
         * @note In future the class might not provide any access to the window handle but instead there will be function
         *       that will provide the specific information from the window. Example 'glfwWindowShouldClose()' we can implement a function 
         *       that calls this function and returns the result only downside of this is redundancy and more function calls.
         */  
		GLFWwindow* GetGLFW_Window()const { return this->m_glfwWindow; }
        /**
         * @brief Returns true if the window was successfully initialize.
         * 
         * @returns Returns true if the window was successfully initialize otherwise false.
         */
		bool IsWinContextInitialized() { return  m_glfwWindow != nullptr;}
        /**
         * @brief Sets the window onEvent callback function.
         *
         * @param onEventFunc The on event callback function that the window should use.
         */
        void SetOnEventCallback(const std::function<void(Event&)>& onEventFunc) { m_onEvent = onEventFunc; }
	private:
        Size2D m_windowSize{}; ///< Current window size.
		GLFWwindow* m_glfwWindow = nullptr; ///< GLFW window handle.
		const char* m_winTitle; ///< Window title/name.
		std::function<void(Event&)> m_onEvent; ///< Window on event callback function.
		WindowState_ windowState = WindowState_kWindowStateNormal;	///< Current window state.
 	private:
        /**
         * @brief Binds the GLFW window callbacks to our Window class function callbacks.
         *
         * Using the 'glfwGetWindowUserPointer' this function binds every GLFW window callback to 
         * our class callback. See this function implementation for more details.
         */
		void SetWindowCallback();
        /**
         * @brief Resolves a mouse position callback.
         * 
         *  Resolves the event by sending an application event(MousePositionEvent)
         * 
         * From the current GLFW docs it states:
         * The callback functions receives the cursor position, measured in screen coordinates 
         * but relative to the top-left corner of the window content area. 
         * On platforms that provide it, the full sub-pixel cursor position is passed on.
         * This information might be outdated see the docs for more info: https://www.glfw.org/docs/3.3/input_guide.html#cursor_pos
         * 
         * 
         * @param window The GLFW window handle.
         * @pram xpos Mouse screen X coordinate.
         * @pram ypos Mouse screen Y coordinate. 
         */
		void glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        /**
         * @brief Resolves a mouse button callback.
         * 
         *  Resolves the event by sending an application event(MouseKeyEvent)
         * 
         * @param window The GLFW window handle.
         * @param button The mouse button witch had it state changed.
         * @param action The mouse state change pressed,release or held.
         * @param mods   Was the key pressed with shift,alt etc.(behavior altering keys).
         */
		void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        /**
         * @brief Resolves a scroll callback.
         * 
         * Resolves the event by sending an application event(ScrollPositionChanged).
         * 
         * @param window The GLFW window handle.
         * @param xoffset For a normal mouse wheel use this to get the offset.
         * @param yoffset See the GLFW docs for more info: https://www.glfw.org/docs/3.3/input_guide.html#scrolling
         */
		void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        /**
         * @brief Resolves a keyboard key action callback.
         * 
         * This function listens for keyboard action press,release etc. and resolves it by sending an application event(KeyboardKeyEvent).
         * 
         * @param window The GLFW window handle.
         * @param key The keyboard keycode witch had it state changed.
         * @param scancode The platform specific Scancodes currently not in use see GLFW  docs for more info:https://www.glfw.org/docs/3.3/input_guide.html#input_key
         * @param action The keyboard state change pressed,release or held.
         * @param mods Was the key pressed with shift,alt etc.(behavior altering keys).
         */
		void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        /**
         * @brief Resolves a window size callback.
         * 
         * @param window The GLFW window handle.
         * @param width The new window width.
         * @param height The new window height
         */
		void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
	};
}
