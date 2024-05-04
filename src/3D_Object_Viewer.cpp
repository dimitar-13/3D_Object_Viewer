#include <iostream>
#include<imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#define WIN_WIDTH 700
#define WIN_HEIGHT 700
#define WIN_TITLE "Object loader"
glm::vec3 testVec = { 1,3,4 };
int InitImGui(GLFWwindow* win);
int main()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "[ERROR]:GLFW failed to initilize." << '\n';
		return -1;
    }
	GLFWwindow* mainWin = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT,WIN_TITLE,NULL,NULL);
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
		std::cout << "[ERROR]:GLEW failed to initilize." << '\n';
		return -3;
	}
	glewExperimental = GL_TRUE;
	InitImGui(mainWin);

	bool isWireframeOn = false;
	float xPos = 1.0f;
	while (!glfwWindowShouldClose(mainWin))
	{
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//Render

		//
		ImGui::Begin("Test");
		ImGui::Text("Model view settings");
		ImGui::InputFloat("x", &xPos);
		ImGui::Checkbox("Wireframe?", &isWireframeOn);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(mainWin);
		glfwPollEvents();
	}
	std::cout << "Done" << '\n';

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

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