#pragma once
#include "pch.h"
#include "WindowHandler.h"
#include "Controls/InputHandler.h"
#include "gpu_side/Framebuffer.h"
#include "AppEvent.h"
#include "Scene/Material.h"
#include "Controls/AppKeyBindings.h"
#include "Logging/App_Logger.h"
#include "Core/SceneViewport.h"
namespace OBJ_Viewer {

	namespace APP_SETTINGS {
		
		enum LightShadingModel
		{
			LIGHT_MODEL_BLIN_PHONG = 0,
			LIGHT_MODEL_TOON_SHADING = 1,
			LIGHT_MODEL_RIM_SHADING = 2,
			LIGHT_MODEL_RIM_AND_TOON_SHADING = 3,
			LIGHT_MODEL_UKNOWN
		};

		enum RenderingMode
		{
			RENDER_MODE_UKNOWN,
			RENDER_MODE_WIREFRAME,
			RENDER_MODE_SOLID_COLOR,
			RENDER_MODE_LIGHT,
			RENDER_MODE_UV,
			RENDER_MODE_INDIVIDUAL_TEXTURES
		};

		struct GridData
		{
			float gridScale = 1.;
			glm::vec3 gridLineColor = glm::vec3(.5);
			bool isAxisShaded = true;
		};
		struct DirectionalLight
		{
			glm::vec3 direction = glm::vec3(0);
			float padding1;
			glm::vec3 color = glm::vec3(0);
			float padding2;
		};
		constexpr uint8_t MAX_LIGHT_COUNT = 4;
		constexpr size_t SIZE_OF_LIGHT_IN_BYTES = sizeof(DirectionalLight);

		struct WireFrameSettings
		{
			float lineThickness = 1.f;
			glm::vec3 lineColor = glm::vec3(0, 1, 0);
			bool isPointRenderingOn = false;
		};
		struct SceneLightInfo
		{
			int lightCount = 1;
			std::array<DirectionalLight, MAX_LIGHT_COUNT>lights = {
				DirectionalLight{glm::vec3(1),0,glm::vec3(1),0},
				DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
				DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
				DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
			};
			LightShadingModel currentLightModel = LIGHT_MODEL_BLIN_PHONG;
		};
		struct UV_ViewAppSetting
		{
			float UV_scaleFactor = 58.0f;
		};
		struct RenderStateSettings
		{
			bool m_isWireGridOn = false;
			bool m_isSkyboxOn = false;
			bool m_EnableAA = true;
			RenderingMode m_currentRenderingMode = RENDER_MODE_SOLID_COLOR;
			glm::vec3 m_colorRenderingColor = glm::vec3(1);
			MaterialFlags m_MaterialFlags = FLAGS_ALL;
			bool isCurrentProjectionPerspective = true;

			bool m_isUniformScale = true;
			MaterialTextures m_curentIndividualTexture = MATERIAL_TEXTURE_ALBEDO;
			UV_ViewAppSetting m_uvViewSettings;
			bool m_disableFBXLoading = true;
			GridData m_gridData;
			SceneLightInfo lightInfo;
			WireFrameSettings wireframeSettings;
		};
		enum BaseMeshType
		{
			MESH_TYPE_CUBE = 0,
			MESH_TYPE_SPHERE = 1,
			MESH_TYPE_TORUS = 2
		};
	}

	class RenderingCoordinator;
	class Application
	{
	public:
		Application(Window& appWindow);
		void AppStartRenderLoop();
		APP_SETTINGS::RenderStateSettings& GetScene_RefSettings() { return m_appStetting; }
		bool isAppInitStatusSuccess()const { return m_appInitStatusSuccsess; }
		bool isUIHidden()const { return m_isUIHidden; }
		InputHandler& GetGlobalInputHandler() { return *m_inputHandler; }
		Window& GetGlobalAppWindow() { return m_window; }
		Framebuffer& GetSceneFrameBuffer() { return m_sceneFramebuffer; }
		SceneViewport GetSceneViewport()const { return m_sceneViewport; }
		const SceneViewport& GetSceneViewport_ConstRef()const { return m_sceneViewport; }
		void AddEventListener(std::weak_ptr<Listener> listener) { m_eventListeners.push_back(listener); }
		void UpdateSceneViewport(const Viewport& newViewport) { m_sceneViewport.UpdateSceneViewport(newViewport); ResizeBuffer(m_sceneViewport.GetViewportSize()); }
		std::function<void(Event&)> GetOnAppEventCallback() { return std::bind(&Application::OnEvent, this, std::placeholders::_1);}
		~Application();
	private:
		void InitImGui();
		void ResizeBuffer(Size2D newSize);
		void OnEvent(Event& winEvent);
		void OnAppKeyBindPressed(KeyboardKeyEvent& e);
	private:
		APP_SETTINGS::RenderStateSettings m_appStetting;
		bool m_isUIHidden = false;
		Window& m_window;
		std::shared_ptr<InputHandler> m_inputHandler;
		Framebuffer m_sceneFramebuffer;
		std::shared_ptr<RenderingCoordinator> m_appRenderingCoordinator;
		std::vector<std::weak_ptr<Listener>> m_eventListeners;
		SceneViewport m_sceneViewport;
		bool m_appInitStatusSuccsess = false;
	};
}

