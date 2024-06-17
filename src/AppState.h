#pragma once
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"ModelLoader.h"
#include"Rendering/ShaderClass.h"
#include"Scene/Camera.h"
#include"WindowHandler.h"
#include <nfd.h>
#include"ShaderPath.h"
#include<memory>
#include"Rendering/Framebuffer.h"
#include<array>
namespace OBJ_Viewer {
constexpr int MAX_LIGHT_COUNT = 4;

	struct GridData
	{
		float gridScale = 1.;
		glm::vec3 gridLineColor = glm::vec3(.5);
		bool isAxisShaded = true;
	};
	struct DirectionalLight
	{
		glm::vec3 direction = glm::vec3(0,-1,0);
		glm::vec3 color = glm::vec3(1);
	};
	struct SceneLightInfo
	{
		int lightCount = 1;
		std::array<DirectionalLight, MAX_LIGHT_COUNT>lights;
	};
	struct RenderStateSettings
	{
		bool m_isWireFrameRenderingOn =false;
		bool m_isSkyboxOn = false ;
		bool m_isWireGridOn = false;
		bool m_isRenderingLightOn = false;

		bool m_isRenderAlbedoTextureOn = true;
		bool m_isRenderSpecularTextureOn = true;
		bool m_isRenderNormalTextureOn = true;
		bool m_isRenderAmbientOcclusionTextureOn = true;
		GridData m_gridData;
		SceneLightInfo lightInfo;
	};
	enum BaseMeshType
	{
		MESH_TYPE_CUBE = 0,
		MESH_TYPE_SPHERE = 1,
		MESH_TYPE_TORUS = 2
	};
	class AppState
	{
	public:
		AppState(Window* pAppWindow,InputHandler& pInputHandler,BaseMeshType typeOfDefaultMeshToLoad = MESH_TYPE_CUBE);
		Model& GetSceneModel() { return *m_currentlyLoadedModel; }
		RenderStateSettings& GetScene_RefSettings() { return m_rendererStateSettings; }
		const Framebuffer& GetSceneFrameBuffer()const { return m_sceneFramebuffer; }
		InputHandler& GetGlobalInputHandler() { return m_pInputHandler; }
		Window& GetGlobalAppWindow() { return *m_windowHandler; }
		void ResizeBuffer(int newWidht, int newHeight);
	private:
	private:
		std::shared_ptr<Model> m_currentlyLoadedModel;
		Window* m_windowHandler = nullptr;
		RenderStateSettings m_rendererStateSettings;
		Framebuffer m_sceneFramebuffer;
		InputHandler& m_pInputHandler;
	};
}

