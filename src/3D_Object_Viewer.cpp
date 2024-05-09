#include <iostream>
#include<imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"RenderingSettings.h"
#define WIN_WIDTH 1500
#define WIN_HEIGHT 1500
#define WIN_TITLE "Object loader"

int InitImGui(GLFWwindow* win);
void freeData();
GLFWwindow* mainWin;
int main()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "[ERROR]:GLFW failed to initialize." << '\n';
		return -1;
    }
	mainWin = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT,WIN_TITLE,NULL,NULL);
	if (mainWin == NULL)
	{
		glfwTerminate();
		std::cout << "[ERROR]:GLFW failed to create window." << '\n';
		return -2;
	}
	glfwMakeContextCurrent(mainWin);

	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(mainWin);
		glfwTerminate();
		std::cout << "[ERROR]:GLEW failed to initialize." << '\n';
		return -3;
	}
	glewExperimental = GL_TRUE;
	InitImGui(mainWin);

	OBJ_Viewer::RenderingSettings::RenderLoop(mainWin);
	return 1;

}
int InitImGui(GLFWwindow* win)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	return 1;
}

void freeData()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(mainWin);
	glfwTerminate();
}
