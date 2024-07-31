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
		
		enum LightShadingModel_
		{
			LightShadingModel_kBlinPhong = 0,
			LightShadingModel_kToonShading = 1,
			LightShadingModel_kRimShading = 2,
			LightShadingModel_kRim_and_ToonShading = 3,
			LightShadingModel_kUknown
		};

		enum RenderingMode_
		{
			RenderingMode_kUknown,
			RenderingMode_kWireframe,
			RenderingMode_kSolidColor,
			RenderingMode_kLight,
			RenderingMode_kUV,
			RenderingMode_kIndividualTexture,
			RenderingMode_kNormalOrientation
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
			LightShadingModel_ currentLightModel = LightShadingModel_kBlinPhong;
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
			RenderingMode_ m_currentRenderingMode = RenderingMode_kSolidColor;
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
		enum BaseMeshType_
		{
			BaseMeshType_kCube = 0,
			BaseMeshType_kSphere = 1,
			BaseMeshType_kTorus = 2
		};
	}

	class RenderingCoordinator;
	class Application
	{
	public:
		Application(Window& appWindow);
		void AppStartRenderLoop();
		APP_SETTINGS::RenderStateSettings& GetScene_RefSettings() { return m_appStetting; }
		bool isFBXLoadingDisabled()const { return m_appStetting.m_disableFBXLoading; }
		bool isAppInitStatusSuccess()const { return m_appInitStatusSuccsess; }
		bool isUIHidden()const { return m_isUIHidden; }
		InputHandler& GetGlobalInputHandler() { return *m_inputHandler; }
		Window& GetGlobalAppWindow() { return m_window; }
		SceneViewport GetSceneViewport()const { return m_sceneViewport; }
		const SceneViewport& GetSceneViewport_ConstRef()const { return m_sceneViewport; }
		void AddEventListener(std::weak_ptr<Listener> listener) { m_eventListeners.push_back(listener); }
		void SubmitSceneViewportSize(const Viewport& newViewport);
		void SubmitEvent(Event& event) { OnEvent(event); }
		~Application();
	private:
		void InitImGui();
		void OnEvent(Event& winEvent);
		void OnAppKeyBindPressed(KeyboardKeyEvent& e);
	private:
		APP_SETTINGS::RenderStateSettings m_appStetting;
		bool m_isUIHidden = false;
		Window& m_window;
		std::shared_ptr<InputHandler> m_inputHandler;
		std::shared_ptr<RenderingCoordinator> m_appRenderingCoordinator;
		std::vector<std::weak_ptr<Listener>> m_eventListeners;
		SceneViewport m_sceneViewport;
		bool m_appInitStatusSuccsess = false;
	};
}

