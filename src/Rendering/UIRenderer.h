#pragma once
#include <iostream>
#include<imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<GL/glew.h>
#include<functional>
namespace OBJ_Viewer {
	struct RendererSettings;

	class UIRenderer {
	public:
		UIRenderer(ImGuiWindowFlags imguiWindowFlags, ImGuiDockNodeFlags imGuiDockSpaceFlags, RendererSettings* pRendererSettings);
		void RenderUI(GLuint frameBuffer);
		ImVec2 GetSceneViewImgSize()const { return m_sceneViewImgSize; }
		void SetOnModelLoadDialogCallback(std::function<void()> callback) { this->onModelLoadDialogCallback = callback; }
	private:
		std::function<void()> onModelLoadDialogCallback;
		ImGuiWindowFlags m_imGuiWindowFlags;
		ImGuiDockNodeFlags m_imgGuiDockSpaceFlags;
		ImVec2 m_sceneViewImgSize;
		RendererSettings* m_pRendererSettings;
	};
}

