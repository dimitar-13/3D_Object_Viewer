#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"WindowHandler.h"
#include"InputHandler.h"
#include"Rendering/Framebuffer.h"
#include<array>
#include"AppEvent.h"
#include"Scene/Material.h"
#include"Controls/AppKeyBindings.h"
namespace OBJ_Viewer {
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
		static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
			GLsizei length, const char* message, const void* userParam);
		RenderStateSettings& GetScene_RefSettings() { return m_appStetting; }
		bool isUIHidden()const { return m_isUIHidden; }
		InputHandler& GetGlobalInputHandler() { return *m_inputHandler; }
		Window& GetGlobalAppWindow() { return *m_window; }
		Framebuffer& GetSceneFrameBuffer() { return *m_sceneFramebuffer; }
		SceneViewport GetSceneViewport()const { return m_sceneViewport; }
		void AddEventListener(std::weak_ptr<Listener> listener) { m_eventListeners.push_back(listener); }
		void UpdateSceneViewport(SceneViewport newViewport) { m_sceneViewport = newViewport; ResizeBuffer(newViewport.width, newViewport.height); }
		std::function<void(Event&)> GetOnAppEventCallback() { return std::bind(&Application::OnEvent, this, std::placeholders::_1);}
		~Application();
	private:
		void InitImGui();
		void ResizeBuffer(int newWidht, int newHeight);
		void OnEvent(Event& winEvent);
		void OnAppKeyBindPressed(KeyboardKeyEvent& e);
	private:
		RenderStateSettings m_appStetting;
		bool m_isUIHidden = false;
		std::unique_ptr<Window> m_window;
		std::shared_ptr<InputHandler> m_inputHandler;
		std::unique_ptr<Framebuffer> m_sceneFramebuffer;
		std::unique_ptr<RenderingCoordinator> m_appRenderingCoordinator;
		std::vector<std::weak_ptr<Listener>> m_eventListeners;

		SceneViewport m_sceneViewport;
	};
}

