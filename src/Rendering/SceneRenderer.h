#pragma once
#include<GL/glew.h>
#include"Scene/Camera.h"
#include"Scene/Model.h"
#include"ShaderClass.h"
#include<memory>
#include"Scene/Skybox.h"
#include"Application.h"
#include"RenderingMediator.h"
#include"Events.h"
#include"Renderer.h"
namespace OBJ_Viewer {
	class SceneRenderer : public Listener
	{
	public:
		SceneRenderer(Application& app,std::shared_ptr<RenderingMediator> mediator);
		~SceneRenderer();
		/// <summary>
		/// Renders a scene using the provided settings to a framebuffer.
		/// </summary>
		/// <param name="renderSettings">The instruction on how to render the scene. </param>
		/// <param name="outputFrameBuffer">The output framebuffer if none provided is understood to be the default provided by GLFW.</param>
		void RenderScene(const RenderStateSettings& renderSettings,Framebuffer* outputFrameBuffer = nullptr);
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

		void RenderGrid(const GridData& appGridData);
		void SetUpShaderForLightRendering(const Mesh& mesh, MaterialFlags materialFlags, SceneLightInfo lightInfo);
		void SetUpForWireframeRendering(const Mesh& mesh,const WireFrameSettings& wireframeAppSettings);
		void PostProcessScene(bool doFXAA = true);
	private:
		std::shared_ptr<Camera> m_sceneCamera;
		std::shared_ptr<Model> m_sceneModel;
		std::shared_ptr<Skybox> m_sceneSkybox;
		Framebuffer m_multiSampleSceneFrameBuffer;
		Framebuffer m_intermidiateFramebuffer;

		VertexAttributeObject m_screenQuad;

		ShaderClass m_clearColorShader;
		ShaderClass m_gridShader;
		ShaderClass m_skyboxShader;
		ShaderClass m_lightShader;
		ShaderClass m_materialShader;
		ShaderClass m_wireframeShader;
		ShaderClass m_wireframePointShader;
		ShaderClass m_UVShader;
		ShaderClass m_singleTextureShader;
		ShaderClass m_postProcessingShader;

		std::shared_ptr<RenderingMediator> m_renderingMediator;
		Application& m_app;
		Renderer m_mainRenderer;
		UniformBuffer m_uniformMatrixBuffer;
		UniformBuffer m_uniformLightBuffer;	
	};
}

