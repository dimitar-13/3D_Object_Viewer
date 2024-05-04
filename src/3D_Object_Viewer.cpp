#include <iostream>
#include<imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#define WIN_WIDTH 1500
#define WIN_HEIGHT 1500
#define WIN_TITLE "Object loader"
glm::vec3 testVec = { 1,3,4 };
int InitImGui(GLFWwindow* win);
int main()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "[ERROR]:GLFW failed to initialize." << '\n';
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
		std::cout << "[ERROR]:GLEW failed to initialize." << '\n';
		return -3;
	}
	glewExperimental = GL_TRUE;
	InitImGui(mainWin);

	bool isWireframeOn = false;
	float xPos = 1.0f;
	constexpr int imguiWinSize = 300;
	glm::vec3 position = { 0,0,0 };
	glm::vec3 scale = { 1,1,1 };
	glm::vec3 rotation = { 0,0,0 };

	uint32_t vertexCount = 4050, triangleCount = 2323, faceCount = 23232;

	while (!glfwWindowShouldClose(mainWin))
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//Render

		//
		ImGui::SetNextWindowPos({ imguiWinSize,WIN_HEIGHT }, ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos({ WIN_WIDTH - ImGui::GetWindowWidth(),0}, ImGuiCond_FirstUseEver);
		//Right panel for model and rendering settings.
		ImGui::Begin("Test");
		ImGui::Text("Model view settings");
		ImGui::InputFloat3("Position",  &position[0]);
		ImGui::InputFloat3("Rotation", &rotation[0]);
		ImGui::InputFloat3("Scale", &scale[0]);
		ImGui::Separator();
		ImGui::Text("Model rendering settings.");
		ImGui::Checkbox("Wireframe?", &isWireframeOn);
		ImGui::Checkbox("Albedo?", &isWireframeOn);
		ImGui::Checkbox("Specular?", &isWireframeOn);
		ImGui::Checkbox("Normals?", &isWireframeOn);
		ImGui::Checkbox("Ambient occlusion?", &isWireframeOn);
		ImGui::End();

		ImGui::Begin("Model data.");
		ImGui::Text("Object triangle count:%d", triangleCount);
		ImGui::Text("Object vertex count:%d", vertexCount);
		ImGui::Text("Object face count:%d", faceCount);
		ImGui::Text("Texture count:%d", 6);
		ImGui::Text("File path %s", "Dummy path");

		ImGui::Separator();
		ImGui::Text("Scene settings.");
		ImGui::Checkbox("UseWorldGrid?", &isWireframeOn);
		ImGui::Checkbox("Enable lights?", &isWireframeOn);
		ImGui::Checkbox("Enable skybox?", &isWireframeOn);
		ImGui::End();
		ImGui::SetNextWindowPos({ imguiWinSize,WIN_HEIGHT }, ImGuiCond_FirstUseEver);
		ImGui::Begin("Loading panel");
		ImGui::Text("Loading stuff here.");
		ImGui::End();
		


		ImGui::SetNextWindowPos({ imguiWinSize,WIN_HEIGHT }, ImGuiCond_FirstUseEver);
		ImGui::Begin("Loading panel");
		ImGui::Text("Loading stuff here.");
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