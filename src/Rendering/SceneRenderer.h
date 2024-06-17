#pragma once
#include<GL/glew.h>
#include"Scene/Camera.h"
#include"Scene/Model.h"
#include"Scene/Skybox.h"
#include"AppState.h"
#include<memory>
namespace OBJ_Viewer {
	class SceneRenderer
	{
	public:
		SceneRenderer(InputHandler& InputHandler,Window& AppWindow);
		~SceneRenderer();
		void RenderScene(RenderStateSettings renderSettings);

		void LoadSkybox(std::vector<char*> paths);
		void LoadModel(char* path);
		void SwapSkyboxFaces(SkyboxFace toSwap, SkyboxFace with);
		std::weak_ptr<Model> GetSceneModel() { return m_sceneModel; }
		std::weak_ptr<Skybox> GetSkyboxModel() { return m_sceneSkybox; }
	private:
		void InitShaders();
		void SetUniformMatrixBuffer()const;
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

		std::unique_ptr<UniformBuffer> m_uniformMatrixBuffer;
		std::unique_ptr<UniformBuffer> m_uniformLightBuffer;


	};
}

