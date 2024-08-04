#pragma once
#include "pch.h"
#include "SceneRenderer.h" 
#include "UI/UILayer.h"
#include "SceneConfigurationSettingsStruct.h"
namespace OBJ_Viewer
{
	class RenderingCoordinator : public Listener{
	public:
		RenderingCoordinator(Application& appState);
		void RenderLoop();
	private:
		void RenderScene();
		void onEventTakeScreenshot(const ScreenshotEvent& e);
		void OnEvent(Event& e) override;
	private:
		Application& m_application;
		std::shared_ptr<UILayer> m_UILayer;
		std::shared_ptr<SceneManager> m_sceneRenderer;
		WindowState_ m_currentWindowState;
		std::future<int> m_saveImgResult;
        APP_SETTINGS::SceneConfigurationSettings m_renderingConfigSettings{};
		// Inherited via Listener
		
	};
}


	

