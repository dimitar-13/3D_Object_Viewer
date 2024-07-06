#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"WindowHandler.h"
#include"InputHandler.h"
#include"Rendering/Framebuffer.h"
#include<array>
#include"AppEvent.h"

namespace OBJ_Viewer {

	enum LightShadingModel
	{
		LIGHT_MODEL_BLIN_PHONG = 0,
		LIGHT_MODEL_TOON_SHADING = 1,
		LIGHT_MODEL_RIM_SHADING = 2,
		LIGHT_MODEL_RIM_AND_TOON_SHADING = 3,
		LIGHT_MODEL_UKNOWN
	};
	constexpr int MAX_LIGHT_COUNT = 4;

	struct GridData
	{
		float gridScale = 1.;
		glm::vec3 gridLineColor = glm::vec3(.5);
		bool isAxisShaded = true;
	};
	struct DirectionalLight
	{
		glm::vec3 direction = glm::vec3(0, 0, 0);
		float padding1;
		glm::vec3 color = glm::vec3(0);
		float padding2;
	};
	struct WireFrameSettings
	{
		float lineThickness = 1.f;
		glm::vec3 lineColor = glm::vec3(0, 1, 0);
		bool isPointRenderingOn = false;
	};
	struct SceneLightInfo
	{
		int lightCount = 1;
		std::array<DirectionalLight, MAX_LIGHT_COUNT>lights;
		LightShadingModel currentLightModel = LIGHT_MODEL_BLIN_PHONG;
	};
	struct UV_ViewAppSetting
	{
		bool isUV_ViewOn = false;
		float UV_scaleFactor = 58.0f;
	};
	struct TextureComposition
	{
		bool isRenderAlbedoTextureOn = true;
		bool isRenderSpecularTextureOn = true;
		bool isRenderNormalTextureOn = true;
		bool isRenderAmbientOcclusionTextureOn = true;
	};
	struct RenderStateSettings
	{
		bool m_isWireFrameRenderingOn = false;
		bool m_isSkyboxOn = false;
		bool m_isWireGridOn = false;
		bool m_isRenderingLightOn = false;

		TextureComposition currentlySetTextures;
		bool isCurrentProjectionPerspective = true;

		bool m_isUniformScale = true;
		bool m_showNormalMapTexture = true;
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
	struct SceneViewport
	{
		int x;
		int y;
		int width;
		int height;
	};
	class RenderingCoordinator;
	class Application
	{
	public:
		Application();
		RenderStateSettings& GetScene_RefSettings() { return m_appStetting; }
		InputHandler& GetGlobalInputHandler() { return *m_inputHandler; }
		Window& GetGlobalAppWindow() { return *m_window; }
		const Framebuffer& GetSceneFrameBuffer()const { return *m_sceneFramebuffer; }
		SceneViewport GetSceneViewport()const { return m_sceneViewport; }
		void AddEventListener(std::weak_ptr<Listener> listener) { m_eventListeners.push_back(listener); }
		void UpdateSceneViewport(SceneViewport newViewport) { m_sceneViewport = newViewport; ResizeBuffer(newViewport.width, newViewport.height); }
		std::function<void(Event&)> GetOnAppEventCallback() { return std::bind(&Application::OnEvent, this, std::placeholders::_1);}
		~Application();
	private:
		void InitImGui();
		void ResizeBuffer(int newWidht, int newHeight);
		void OnEvent(Event& winEvent);
	private:
		RenderStateSettings m_appStetting;
		std::unique_ptr<Window> m_window;
		std::unique_ptr<InputHandler> m_inputHandler;
		std::unique_ptr<Framebuffer> m_sceneFramebuffer;
		std::unique_ptr<RenderingCoordinator> m_appRenderingCoordinator;
		std::vector<std::weak_ptr<Listener>> m_eventListeners;

		SceneViewport m_sceneViewport;
	};
}

