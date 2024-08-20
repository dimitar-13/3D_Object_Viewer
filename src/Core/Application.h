#pragma once
#include "pch.h"
#include "WindowHandler.h"
#include "Controls/InputHandler.h"
#include "gpu_side/Framebuffer.h"
#include "AppEvent.h"
#include "Scene/Material.h"
#include "Controls/AppKeyBindings.h"
#include "Logging/App_Logger.h"
#include "Core/SceneViewport.h"

namespace OBJ_Viewer {
	class RenderingCoordinator;
    /**
     * @brief Class representing the application.
     *
     * The `Application` class is the main entry point and management class for the application. It encapsulates the application
     * window and viewport, serving as a central hub for accessing global data and managing key application components. The class
     * provides access to other critical components such as `SceneViewport`, `Window`, and `InputHandler`, which are essential for
     * the operation of the application. 
     *
     */
	class Application
	{
	public:
        /**
         * @brief Initialize the application class.
         *
         * The constructor initialize classes used by the application.
         * - The InputHandler.
         * - RenderingCoordinator
         * It also sets the 'RenderingCoordinator' and 'InputHandler' as a event listeners.
         * The function also sets the glViewport to the window size.
         * @note In future we might record the last window size this will allow us to get the viewport size from the imgui ini file
         *       and save us 1 resize.
         */
        Application(Window& appWindow);
        /**
         * @brief Starts the rendering loop.
         *
         * This function calls the RenderingCoordinator that is where the rendering loop is.
         */
		void AppStartRenderLoop();
        /**
         * @brief Returns true if the application was successfully initialized.
         * @returns  True if the application was successfully initialized and false otherwise. 
         */
		bool IsAppInitStatusSuccess()const { return m_appInitStatusSuccess; }
        /**
         * @brief Returns true if the UI is hidden.
         *
         * @returns Returns true if the UI is hidden and false if not.
         * @note this function might go in the RenderingCoordinator since the rest of the application don't care if UI is visible or not.
         */
		bool IsUIHidden()const { return m_isUIHidden; }
        /**
         * @brief Retrieves the global application InputHandler.
         *
         * This function returns a reference to the global `InputHandler` instance used by the application. The `InputHandler`
         * manages user input and interactions, providing access to input-related events and states.
         *
         * @return Reference to the global `InputHandler`.
         */
		InputHandler& GetGlobalInputHandler() { return *m_inputHandler; }
        /**
         * @brief Retrieves the global application window.
         *
         * This function returns a readonly reference to the `Window` instance representing the application window.
         *
         * @return Readonly reference to the `Window` instance.
         */
		const Window& GetGlobalAppWindow()const { return m_window; }
        /**
         * @brief Retrieves the scene viewport settings.
         *
         * This function returns a readonly reference to the `SceneViewport` instance.
         *
         * @return Readonly reference to the `SceneViewport` instance.
         */
		const SceneViewport& GetSceneViewport()const { return m_sceneViewport; }
        /**
         * @brief Retrieves the scene viewport settings.
         *
         * This function returns a reference to the `SceneViewport` instance.
         *
         * @return Reference to the `SceneViewport` instance.
         */
        SceneViewport& GetSceneViewport() { return m_sceneViewport; }
        /**
         * @brief Adds a new event listener to the application.
         *
         * This function registers a new event listener by adding a `std::weak_ptr` to an object that implements the
         * `Listener` interface to the application's internal list of event listeners. This allows the application to notify
         * registered listeners of various events.
         *
         * @param listener A `std::weak_ptr` to an object that implements the `Listener` interface.
         */
		void AddEventListener(std::weak_ptr<Listener> listener) { m_eventListeners.push_back(listener); }
        /**
         * @brief Submits a new viewport size to the application.
         *
         * This function updates the application with a new viewport size. If the new size is different from the current size,
         * it will trigger a resize of the viewport by calling `glViewport` and will invoke a viewport resize event.
         * If the size remains the same, no changes will be made to the viewport and no event will be invoked.
         *
         * @param newViewport The new `Viewport` dimensions to be applied.
         */
		void SubmitSceneViewportSize(const Viewport& newViewport);
        /**
         * @brief Submits a global application event.
         *
         * This function submits an event to the application, allowing it to be handled by other classes listening to events.
         *
         * @param event Reference to the polymorphic object implementing 'Event' interface 
         *              type that contains the details of the event to be submitted.
         */
		void SubmitEvent(Event& event) { OnEvent(event); }
	private:
        /**
         * @brief Fires an event and notifies every event listener.
         * 
         * This function fires an event. And notifies every valid listener.
         * Where a valid listener is one that is not expired or deleted.
         * For now the event firing is done in order meaning the first event listener will resolve the event while the others wait for it.
         * In the future I might look into async events and event manager that sends only relevant events.
         * 
         * @param winEvent Created event information.
         */
		void OnEvent(Event& winEvent);
        /**
         * @brief Currently used for the HideUI key event.
         *
         * @note This will most likely be removed in the future if I continue to work on this project.
         */
		void OnAppKeyBindPressed(KeyboardKeyEvent& e);
	private:
		bool m_isUIHidden = false;///< UI visibility state
		Window& m_window; ///< Application window.
		std::shared_ptr<InputHandler> m_inputHandler; ///< Global application input handler.
		std::shared_ptr<RenderingCoordinator> m_appRenderingCoordinator; ///< Global application rendering coordinator.
		std::vector<std::weak_ptr<Listener>> m_eventListeners; ///< Application event listeners vector.
		SceneViewport m_sceneViewport; ///< Application rendering viewport(not the window but the square/box that is in between the UI).
		bool m_appInitStatusSuccess = false; ///< Application initlize satatus.
	};
}

