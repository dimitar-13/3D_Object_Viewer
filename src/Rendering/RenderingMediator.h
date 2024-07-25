#pragma once
#include "Scene/Skybox.h"
#include "Scene/Model.h"
#include "pch.h"

namespace OBJ_Viewer {
	class RenderingMediator
	{
	public:
		RenderingMediator() = default;

		void SetSkybox(std::shared_ptr<Skybox> newSkybox) { m_skybox = newSkybox; }
		void SetSceneModel(std::shared_ptr<Model> newModel) { m_sceneModel = newModel; }

		const std::weak_ptr<Skybox> GetSkybox()const { return std::weak_ptr(m_skybox); }
		const std::weak_ptr<Model> GetModel()const { return std::weak_ptr(m_sceneModel); }
	private:
		std::shared_ptr<Skybox> m_skybox;
		std::shared_ptr<Model> m_sceneModel;

	};
}

