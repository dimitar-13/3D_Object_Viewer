#pragma once
#include "Scene/Skybox.h"
#include "Scene/Model.h"
#include "pch.h"
#include "Scene/MaterialRegistry.h"
namespace OBJ_Viewer {
	class RenderingMediator
	{
	public:
		RenderingMediator() = default;

		void SetSkybox(std::shared_ptr<Skybox> newSkybox) { m_skybox = newSkybox; }
		void SetSceneModel(std::shared_ptr<Model> newModel) { m_sceneModel = newModel; }
		void SetSceneMaterialRegistry(std::shared_ptr<MaterialRegistry> newRegistry) { m_sceneRegistry = newRegistry; }

		const std::weak_ptr<Skybox> GetSkybox()const { return std::weak_ptr(m_skybox); }
		const std::weak_ptr<Model> GetModel()const { return std::weak_ptr(m_sceneModel); }
		const std::weak_ptr<MaterialRegistry> GetMaterialRegistry()const { return std::weak_ptr(m_sceneRegistry); }

	private:
		std::shared_ptr<Skybox> m_skybox;
		std::shared_ptr<Model> m_sceneModel;
		std::shared_ptr<MaterialRegistry> m_sceneRegistry;

	};
}

