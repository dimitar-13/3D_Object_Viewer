#pragma once
#include "pch.h"
#include "SceneRenderer.h" 
#include "UI/UILayer.h"

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
		std::unique_ptr<UILayer> m_UILayer;
		std::shared_ptr<SceneRenderer> m_sceneRenderer;
		WindowState m_currentWindowState;
		std::future<int> m_saveImgResult;
		// Inherited via Listener
		
	};
}


	

