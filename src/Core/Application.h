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
	class Application
	{
	public:
        Application(Window& appWindow);
		void AppStartRenderLoop();
		bool isAppInitStatusSuccess()const { return m_appInitStatusSuccsess; }
		bool isUIHidden()const { return m_isUIHidden; }
		InputHandler& GetGlobalInputHandler() { return *m_inputHandler; }
		const Window& GetGlobalAppWindow() { return m_window; }
		const SceneViewport& GetSceneViewport()const { return m_sceneViewport; }
        SceneViewport& GetSceneViewport() { return m_sceneViewport; }
		void AddEventListener(std::weak_ptr<Listener> listener) { m_eventListeners.push_back(listener); }
		void SubmitSceneViewportSize(const Viewport& newViewport);
		void SubmitEvent(Event& event) { OnEvent(event); }
	private:
		void OnEvent(Event& winEvent);
		void OnAppKeyBindPressed(KeyboardKeyEvent& e);
	private:
		bool m_isUIHidden = false;
		Window& m_window;
		std::shared_ptr<InputHandler> m_inputHandler;
		std::shared_ptr<RenderingCoordinator> m_appRenderingCoordinator;
		std::vector<std::weak_ptr<Listener>> m_eventListeners;
		SceneViewport m_sceneViewport;
		bool m_appInitStatusSuccsess = false;
	};
}

