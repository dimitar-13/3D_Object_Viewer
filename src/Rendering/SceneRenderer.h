#pragma once
#include "pch.h"
#include "Scene/Camera.h"
#include "Scene/Model.h"
#include "gpu_side/ShaderClass.h"
#include "Scene/Skybox.h"
#include "Core/Application.h"
#include "Core/Events.h"
#include "Scene/MaterialRegistry.h"
#include "Renderer.h"
#include "Rendering/ShaderLibrary.h"
#include "Helpers/ModelLoader.h"
#include "SceneConfigurationSettingsStruct.h"

namespace OBJ_Viewer {
	class SceneManager : public Listener
	{
	public:
		SceneManager(Application& app);
		~SceneManager();
		/// <summary>
		/// Renders a scene using the provided settings to a framebuffer.
		/// </summary>
		/// <param name="renderSettings">The instruction on how to render the scene. </param>
		/// <param name="outputFrameBuffer">The output framebuffer if none provided is understood to be the default provided by GLFW.</param>
		void RenderScene(const APP_SETTINGS::SceneConfigurationSettings& renderSettings,Framebuffer* outputFrameBuffer = nullptr);
		std::weak_ptr<Model> GetSceneModel() { return m_sceneModel; }
		std::weak_ptr<Skybox> GetSkyboxModel() { return m_sceneSkybox; }
        std::weak_ptr<MaterialRegistry> GetSceneRegistry() { return m_sceneRegistry; }
	private:
		void SetUpUniformBuffers();
		void SetUniformMatrixBuffer();
		// Inherited via AppEventListener
		void OnEvent(Event& e)override;
		void OnSkyboxLoadEvent(EventOnSkyboxLoaded& e);
		void OnSceneLoadEvent(EventOnSceneLoad& e);

		void LoadSkybox(std::vector<std::string>& skyboxTextrePaths);
		void LoadSceneFile(const std::string& path,LoadModelFileType_ model_file_format);
		void PostProcessScene(bool doFXAA = true);
	private:
#pragma region Scene render objects
		std::shared_ptr<Camera> m_sceneCamera;
		std::shared_ptr<Model> m_sceneModel;
		std::shared_ptr<Skybox> m_sceneSkybox;
		std::shared_ptr<MaterialRegistry> m_sceneRegistry;
		VertexAttributeObject m_screenQuad;
#pragma endregion

#pragma region Scene buffers
		Framebuffer m_multiSampleSceneFrameBuffer;
		Framebuffer m_intermidiateFramebuffer;
		UniformBuffer m_uniformMatrixBuffer;
		UniformBuffer m_uniformLightBuffer;	
#pragma endregion

		ShaderLibrary m_shaderLib;
		Application& m_app;
		Renderer m_mainRenderer;
	};
}

