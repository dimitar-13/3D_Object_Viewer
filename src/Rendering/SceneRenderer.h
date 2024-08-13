#pragma once
#include "pch.h"
#include "Scene/Camera.h"
#include "Scene/Model.h"
#include "gpu_side/ShaderClass.h"
#include "Scene/Skybox.h"
#include "Core/Application.h"
#include "Core/WindowEvents.h"
#include "Scene/MaterialRegistry.h"
#include "Renderer.h"
#include "Rendering/ShaderLibrary.h"
#include "Helpers/ModelLoader.h"
#include "SceneConfigurationSettingsStruct.h"

namespace OBJ_Viewer {

    /*
     * Scene Anti-aliasing(AA):
     *
     * We AA using 2 AA methods MSAA + FXAA.
     * First we do MSAA using the multi-sample buffer the sample count can be adjusted but as of now is 8.
     * We use the MSAA only for the mesh rendering because the grid and skybox are entities that don't benefit from this AA method.
     * After we use MSAA we copy it's color + depth info to an intermidiate framebuffer and render the grid and skybox.
     * At the end we do the FXAA AA using the intermidiate framebuffer.
     *
     * @note Although we do FXAA on the gird the grid itself is already AA but FXAA is a post-process algorithm so we don't
     *       have that much of a performance penalty for AA the gird.
     */

    /**
     * @brief Class for rendering the entire scene.
     *
     * Unlike the 'Render' class this class knows the bigger picture. The class manages all of the scene related 
     * information such as: Skybox, Model, Uniform buffer objects (UBO) etc.
     * 
     * The class knows the rendering order and know witch rendering function to use from the 'Render' class
     * for the task. The class also do scene Anti-aliasing(AA) the scene if enabled using 2 framebuffers(see the scene AA for more info 
     * on how we do AA every frame ^^^^.)
     *                                                       
     * The class works with all of the scene related classes such as Model,Skybox,Camera and MaterialRegistry
     * and is responsible to handle the model and skybox event changes.
     * 
     * @note One could argue the purpose of the class is not clear witch is a valid point.
     *       The class can be split in some subclasses most notably sub managers that clearly express their responsibility
     *       Currently this in my opinion is not a problem but in the future if this class become harder to maintain it will be 
     *       split into subclasses.
     * 
     */
	class SceneManager : public Listener
	{
	public:
        /**
         * @brief Initializes the application scene.
         *
         * Constructor initializes all of the application scene related data:
         * - Uniform buffer objects(UBO): Initializes the light UBO and the matrix UBO.
         * - Model: Creates the default model of the application(The creation code is in 'Helpers/ModelGenerator.h'
         *                                                       the class only invokes the creation and stores the result).
         * - MaterialRegistry: Creates a default registry.
         * - Camera: Invokes camera constructor and adds the camera as event Listener.
         * 
         * @param app The application ref used across the application.
         */
		SceneManager(Application& app);
        /**
         * @brief Function that renders 1 frame from the application scene using the provided configurations.
         *
         * The function renders a frame of the application scene using the proved configurations.
         * The function can output the rendered frame to a custom framebuffer if the outputFrameBuffer is proved 
         * if no framebuffer is provided is understood to use the default OpenGL framebuffer.
         * 
         * The render can be summarized a this:
         * - Render the model(mesh by mesh) using the proved configuration.
         * - Render grid if enabled.
         * - Render skybox if enabled.
         * - Do post-process (this consist of AA(FXAA + MSAA) and gamma correction).
         *
         * @param renderSettings The configuration used to determine how and what to render.
         * @param outputFrameBuffer The render to framebuffer if none provided the function will use the default OpenGL framebuffer.
         */
		void RenderScene(const APP_SETTINGS::SceneConfigurationSettings& renderSettings,
            Framebuffer* outputFrameBuffer = nullptr);
        /**
         * @brief Retrieves a weak pointer to the scene model.
         *
         * This method provides access to the scene model as a `std::weak_ptr`. This allows external code to access the model
         * without affecting its reference count, preventing potential ownership issues.
         *
         * @return std::weak_ptr<Model> A weak pointer to the scene model.
         */
		std::weak_ptr<Model> GetSceneModel() { return m_sceneModel; }
        /**
         * @brief Retrieves a weak pointer to the scene skybox.
         *
         * This method provides access to the scene skybox as a `std::weak_ptr`. This allows external code to access the skybox
         * without affecting its reference count, helping to avoid unintended ownership changes.
         *
         * @return std::weak_ptr<Skybox> A weak pointer to the scene skybox.
         */
		std::weak_ptr<Skybox> GetSkyboxModel() { return m_sceneSkybox; }
        /**
         * @brief Retrieves a weak pointer to the material registry.
         *
         * This method provides access to the material registry as a `std::weak_ptr`. This allows external code to access the registry
         * without affecting its reference count, which helps in managing material data without altering its ownership.
         *
         * @return std::weak_ptr<MaterialRegistry> A weak pointer to the material registry.
         */
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

