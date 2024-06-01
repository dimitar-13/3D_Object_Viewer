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
#include"Scene/Skybox.h"
namespace OBJ_Viewer {
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
		AppState(Window* pAppWindow,InputHandler* pInputHandler,BaseMeshType typeOfDefaultMeshToLoad = MESH_TYPE_CUBE);
		const Model& GetSceneModel()const { return *m_currentlyLoadedModel; }
		const Skybox* GetSceneSkybox()const { return m_Skybox.get(); }
		RenderStateSettings* GetScene_pSettings() { return &m_rendererStateSettings; }
		const Framebuffer& GetSceneFrameBuffer()const { return m_sceneFramebuffer; }
		InputHandler* GetGlobalInputHandler() { return m_pInputHandler; }
		Window& GetGlobalAppWindow() { return *m_windowHandler; }
		void LoadModel(char* path);
		void LoadSkybox(std::vector<char*> paths);
		void SwapSkyboxFaces(SkyboxFace toSwap, SkyboxFace with);
		void ResizeBuffer(int newWidht, int newHeight);
	private:
		std::shared_ptr<Model> m_currentlyLoadedModel;
		std::shared_ptr<Skybox> m_Skybox;
		Window* m_windowHandler = nullptr;
		RenderStateSettings m_rendererStateSettings;
		Framebuffer m_sceneFramebuffer;
		InputHandler* m_pInputHandler = nullptr;
	};
}

