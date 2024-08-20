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
        /**
         * @brief Sets up the scene uniform buffers data range and binds them to shaders.
         *
         * This function sets all of the scene uniform buffer objects(UBO) data range.
         * And then it bind them to the shaders.
         */
		void SetUpUniformBuffers();
        /**
         * @brief Updates the matrix uniform buffer object (UBO) with the latest matrix data.
         *
         * This function manages the matrix uniform buffer object (UBO) by updating or setting up its data.
         * It gathers matrix data from the `Model` and `Camera` classes, ensuring that the UBO contains the
         * most current transformation and view matrices. This function is called every frame to keep the UBO
         * updated, which reduces the number of API calls required during rendering.
         */
		void SetUniformMatrixBuffer();
        /**
         * @brief Called by an event notifier when an event occurs.
         *
         * This virtual function, inherited from the `Listener` class, is overridden to handle incoming events.
         * The function identifies the type of event and delegates it to the appropriate handler function for processing.
         *
         * @param e The event object that contains details about the event that occurred.
         */
		void OnEvent(Event& e)override;
        /**
         * @brief Handles the skybox load event by opening a file dialog and validating input for skybox creation.
         *
         * This function is triggered when a skybox load event occurs. It opens a file dialog that allows the user to select
         * multiple image files (e.g., PNG, JPEG, JPG) for the skybox textures. After the user selects the files, the function
         * validates the input by checking if the dialog was successful and proceeds with the skybox creation
         * using the selected files and calling 'LoadSkybox()'.
         *
         * If the dialog was not successful(error or no files selected) the function don't proceed any further.
         * 
         * 
         * @param e The event object containing information related to the skybox load event.
         */
		void OnSkyboxLoadEvent(EventOnSkyboxLoaded& e);
        /**
         * @brief Handles the scene or 3D model load event.
         *
         * This function is triggered when a scene or 3D model load event occurs.
         * It opens a file dialog for the user to select a file and then validates the selection.
         *
         * The validation process includes:
         * - Checking if the dialog result is valid (refer to the `DialogWrapper` class for what valid means.).
         * - Verifying the selected file format. If the file is in FBX format and FBX loading is disabled, the function does not proceed further.
         *
         * If all validations pass, the function proceeds with the actual loading process by passing the file path and format to the `LoadSceneFile()` method.
         * 
         * @param e The event object containing information related to the scene load event.
         */
		void OnSceneLoadEvent(EventOnSceneLoad& e);
        /**
         * @brief Attempts to create a skybox from a set of texture file paths.
         *
         * This function receives a vector of absolute texture file paths and attempts to read and load the textures to create a skybox.
         * If the vector is empty or contains fewer than `Skybox::SKYBOX_FACE_COUNT` paths, the function returns without making any changes.
         *
         * The function processes each texture path in the vector sequentially. If it encounters an invalid texture at any point, it
         * aborts the process and returns without creating or updating the skybox. If all textures are successfully loaded, a new skybox is created,
         * the reference count of the `m_sceneSkybox` shared pointer is reset, and the new skybox is assigned.
         *
         * @param skyboxTextrePaths A vector of strings representing the absolute paths to the texture files.
         */
		void LoadSkybox(std::vector<std::string>& skyboxTextrePaths);
        /**
         * @brief Updates the `m_sceneModel` and `m_sceneRegistry` with a newly loaded 3D model from the specified path.
         *
         * This function utilizes the `ModelLoader` helper class to load a 3D model file (either OBJ or FBX) from the given path.
         * If the file is successfully loaded, the function updates the `m_sceneModel` and `m_sceneRegistry` member variables with the
         * new data. If the loading process fails, the existing member variables remain unchanged.
         *
         * @param path The file path of the 3D model to be loaded.
         * @param model_file_format The format of the 3D model file (e.g., OBJ, FBX).
         */
		void LoadSceneFile(const std::string& path,LoadModelFileType_ model_file_format);
        /**
         * @brief Applies post-processing effects to the final framebuffer image.
         *
         * This function performs two post-processing techniques on the final rendered scene:
         *  - Fast Approximate Anti-Aliasing (FXAA): Reduces aliasing in the scene. This can be toggled on or off.
         *  - Gamma Correction: Adjusts the gamma levels of the scene for correct color representation.
         *
         * The function utilizes a post-process shader to configure and apply the necessary variables for these effects.
         *
         * @param doFXAA A boolean flag to enable or disable FXAA. The default value is `true`.
         */
		void PostProcessScene(bool doFXAA = true);
	private:
#pragma region Scene render objects
		std::shared_ptr<Camera> m_sceneCamera;               ///< 3D scene camera class.
		std::shared_ptr<Model> m_sceneModel;                 ///< Currently loaded 3D scene/model.
		std::shared_ptr<Skybox> m_sceneSkybox;               ///< Currently loaded skybox(is null by default).
		std::shared_ptr<MaterialRegistry> m_sceneRegistry;   ///< Currently loaded 3D scene/model material registry.
		VertexAttributeObject m_screenQuad;                  ///< The full screen quad vertex attribute object used for post-processing and grid drawing.
#pragma endregion                                            
                                                             
#pragma region Scene buffers                                 
		Framebuffer m_multiSampleSceneFrameBuffer;           ///< The multi-sampled framebuffer used for multi-sampled anti-aliasing(MSAA).
		Framebuffer m_PostProcessingFramebuffer;             ///< The framebuffer witch texture is used for post-processing.
		UniformBuffer m_uniformMatrixBuffer;                 ///< The uniform matrix buffer shared across shader containing coordinate space matrices. 
		UniformBuffer m_uniformLightBuffer;	                 ///< The uniform light buffe containing light information.
#pragma endregion
		ShaderLibrary m_shaderLib;                           ///< The application shader library containing all of the relevant shaders.
		Application& m_app;                                  ///< Application ref.
		Renderer m_mainRenderer;                             ///< Main renderer containing the rendering logic.
	};
}

