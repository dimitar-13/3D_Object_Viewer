#pragma once
#include<memory>
#include "SceneRenderer.h" 
#include"UI/UILayer.h"

namespace OBJ_Viewer
{
	class RenderingCoordinator{
	public:
		RenderingCoordinator(AppState* appState);
		void RenderLoop();
	private:
		void RenderScene();
	private:
		AppState* m_appState;
		std::unique_ptr<UILayer> m_UILayer;
		std::shared_ptr<SceneRenderer> m_sceneRenderer;
	};
}


	

