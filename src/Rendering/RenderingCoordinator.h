#pragma once
#include<memory>
#include "SceneRenderer.h" 
#include"UI/UILayer.h"

namespace OBJ_Viewer
{
	class RenderingCoordinator{
	public:
		RenderingCoordinator(Application& appState);
		void RenderLoop();
	private:
		void RenderScene();
	private:
		Application& m_application;
		std::unique_ptr<UILayer> m_UILayer;
		std::shared_ptr<SceneRenderer> m_sceneRenderer;
	};
}


	

