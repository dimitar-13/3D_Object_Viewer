#pragma once
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
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
#include"AppState.h"
namespace OBJ_Viewer {

	class UILayer
	{
	public:
		UILayer(AppState* appState,ImGuiWindowFlags imGuiWindowFlags,
			ImGuiDockNodeFlags imGuiDockSpaceFlags);
		void RenderUI();
	private:
		void LoadModel();
		void LoadSkybox();
		void RenderComboBox(std::string comboLabel, int index);
		void RenderSkyboxSettings(const Skybox* skybox);
	private:
	
		AppState* m_appState;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;
	};
}

