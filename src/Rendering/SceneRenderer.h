#pragma once
#include<GL/glew.h>
#include"Scene/Camera.h"
#include"Scene/Model.h"
#include"ShaderClass.h"
#include<memory>
#include"Scene/Skybox.h"
#include"Application.h"
#include"RenderingMediator.h"
#include"IObserver.h"
namespace OBJ_Viewer {
	class SceneRenderer : public Listener
	{
	public:
		SceneRenderer(Application& app,std::shared_ptr<RenderingMediator> mediator);
		~SceneRenderer();
		void RenderScene(RenderStateSettings renderSettings);	
		void SwapSkyboxFaces(SkyboxFace toSwap, SkyboxFace with);
		std::weak_ptr<Model> GetSceneModel() { return m_sceneModel; }
		std::weak_ptr<Skybox> GetSkyboxModel() { return m_sceneSkybox; }
	private:
		void InitShaders();
		void SetUniformMatrixBuffer()const;
		glm::mat3 ConstructViewportMatrix()const;
		// Inherited via AppEventListener
		void OnEvent(Event& e)override;
		void OnSkyboxLoadEvent(EventOnSkyboxLoaded& e);
		void OnModelLoadEvent(EventOnModelLoaded& e);

		void LoadSkybox(std::vector<std::string>&);
		void LoadModel(const std::string& path);
	private:
		std::shared_ptr<Camera> m_sceneCamera;
		std::shared_ptr<Model> m_sceneModel;
		std::shared_ptr<Skybox> m_sceneSkybox;
		std::unique_ptr<VertexAttributeObject> m_gridVAO;

		std::unique_ptr<ShaderClass> m_clearColorShader;
		std::unique_ptr<ShaderClass> m_gridShader;
		std::unique_ptr<ShaderClass> m_skyboxShader;
		std::unique_ptr<ShaderClass> m_lightShader;
		std::unique_ptr<ShaderClass> m_materialShader;
		std::unique_ptr<ShaderClass> m_wireframeShader;
		std::shared_ptr<RenderingMediator> m_renderingMediator;
		Application& m_app;

		std::unique_ptr<UniformBuffer> m_uniformMatrixBuffer;
		std::unique_ptr<UniformBuffer> m_uniformLightBuffer;	
	};
}

