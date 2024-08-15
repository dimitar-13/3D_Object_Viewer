#pragma once
#include "pch.h"
#include "SceneRenderer.h" 
#include "UI/UILayer.h"
#include "SceneConfigurationSettingsStruct.h"
namespace OBJ_Viewer
{
    /**
     * @brief Class responsible for the render loop and communication between the UI and the scene rendering system.
     *
     * This class is one of the most crucial components of the program, containing the main render loop.
     * It facilitates the interaction between two key classes: 'UILayer' and 'SceneManager'.
     *
     * The class coordinates with the 'UILayer' and 'SceneManager' in each frame, executing two primary tasks:
     * - Collaborates with the 'UILayer' to render the UI and retrieve user input.
     * - Passes the user input from the 'UILayer' to the 'SceneManager', which uses it to determine how to render the scene.
     *
     * The class holds member of type 'APP_SETTINGS::SceneConfigurationSettings' structure, which reference is provided to the UI,
     * allowing users to submit their settings. Afterward, this information is forwarded to the 'SceneManager'.
     *
     * Additionally, this class manages a screenshot event (which may be moved to the 'SceneManager' class in the future).
     * The class also registers the 'UILayer' and 'SceneManager' as event listeners.
     */
	class RenderingCoordinator : public Listener{
	public:
        /**
         * @brief Constructor initializes and registers the 2 main classes UILayer' and 'SceneManager' as event listeners.
         * 
         * The constructor gets an application reference and initiates the 2 main classes UILayer' and 'SceneManager'.
         * These classes are then submitted as event listeners.
         * 
         * @param applicationRef Reference to the main application class.
         */
		RenderingCoordinator(Application& applicationRef);
        /**
         * @brief Function that starts the render loop.
         *
         * This function starts the render loop. Each frame the functions
         * starts by rendering the UI and then the scene. This is done because of 2 reasons.
         * 1) The UI first because we need a viewport size and currently we cant know it without rendering the UI first.
         * 2) We need UI user input to render the scene technically the user wont submit something new on the first frame 
         *    and the 'SceneManager' will receive default values but still it is better this way.
         * 
         * The function also polls the GLFW input/window events.
         * This function also takes care if the UI is hidden and don't render the UI in that case.
         * 
         * This function runs until the window has been closeted.
         * @note If the window state is minimized see 'WindowState_' this function wont render anything it will only poll the GLFW events.
         */
		void RenderLoop();
	private:
        /**
         * @brief Determines witch framebuffer to use for the scene rendering and calls the 'm_SceneManager' for the actual rendering.
         *
         * Based on if the UI is hidden or not this function will use the default or UI framebuffer respectively as a parameter to
         * the render scene in the 'm_SceneManager' member.
         */
		void RenderScene();
        /**
         * @brief Handles the screenshot event.
         *
         * This function handle the screenshot event by resizing the framebuffers and changing some application state then 
         * it renders the scene and extract the pixel data and passes the data to a static function(not in this class) to do the 
         * image saving asynchronously. It reset the application and framebuffers previous state and continues with the render loop.
         * 
         * @note The image saving is asynchronously and the std::future is stored in the 'm_saveImgResult'.
         *       The result of the image saving is not checked or used and even if it fails it still considered as correctly handled event.
         * 
         * @param e The screenshot event containing how to save/take the screenshot.
         */
		void onEventTakeScreenshot(const ScreenshotEvent& e);
        /**
         * @brief Handles incoming events by delegating them to appropriate handlers.
         *
         * This function is inherited from the 'Listener' interface and is responsible for processing events that occur
         * within the application. Depending on the type of event, it delegates the event to specific handling functions
         * designed to manage that particular event.
         *
         * @param e The event object that contains information about the specific event that occurred.
         */
		void OnEvent(Event& e) override;
	private:
		Application& m_application;                                             ///< Application ref.
		std::shared_ptr<UILayer> m_UILayer;                                     ///< Class for rendering the UI and retrieving the information submited by the user.
		std::shared_ptr<SceneManager> m_SceneManager;                           ///< Scene manager representing the 3D scene.
		WindowState_ m_currentWindowState;                                      ///< Current window state see 'WindowState_'.
		std::future<bool> m_saveImgResult;                                      ///< Save image thread result.
        APP_SETTINGS::SceneConfigurationSettings m_renderingConfigSettings{};   ///< The application rendering configuration data struct used by the UI and the render.	
	};
}


	

