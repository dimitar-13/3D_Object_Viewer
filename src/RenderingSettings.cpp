#include "RenderingSettings.h"
#include "VertexAttributeObject.h"
#include"ShaderClass.h"

constexpr float scaleVal = 1 / 1;
//Test objs;
static std::vector<float> vertices = { -0.5f * scaleVal, -0.5f * scaleVal, // bottom left corner
										-0.5f * scaleVal,  0.5f * scaleVal, // top left corner
										 0.5f * scaleVal,  0.5f * scaleVal, // top right corner
										 0.5f * scaleVal, -0.5f * scaleVal, }; // bottom right corner

static std::vector <unsigned int> indices = { 0,1,2, // first triangle (bottom left - top left - top right)
					 0,2,3 }; // second triangle (bottom left - top right - bottom right)


void OBJ_Viewer::RenderingSettings::RenderLoop(GLFWwindow* mainWin)
{
	ShaderClass shader("D:/c++/OpenGl/3D_Object_Viewer/3D_Object_Viewer/Shaders/Shader.glsl");
	VertexAttributeObject vertexAttribObj = { vertices,indices };
	while (!glfwWindowShouldClose(mainWin))
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		shader.UseShader();
		vertexAttribObj.BindBuffer();
		glDrawElements(GL_TRIANGLES, vertexAttribObj.GetIndexCount(), GL_UNSIGNED_INT, NULL);
		vertexAttribObj.UnBind();

		RenderScene();
		RenderImGui();

		glfwSwapBuffers(mainWin);
		glfwPollEvents();
	}

}

void OBJ_Viewer::RenderingSettings::RenderScene()
{
}

void OBJ_Viewer::RenderingSettings::RenderImGui()
{
	//Testing values
	float xPos = 1.0f;
	glm::vec3 position = { 0,0,0 };
	glm::vec3 scale = { 1,1,1 };
	glm::vec3 rotation = { 0,0,0 };

	uint32_t vertexCount = 4050, triangleCount = 2323, faceCount = 23232;

	//Right panel for model and rendering settings.
	ImGui::Begin("Test");
	ImGui::Text("Model view settings");
	ImGui::InputFloat3("Position", &position[0]);
	ImGui::InputFloat3("Rotation", &rotation[0]);
	ImGui::InputFloat3("Scale", &scale[0]);
	ImGui::Separator();
	ImGui::Text("Model rendering settings.");
	ImGui::Checkbox("Wireframe?", &m_isWireFrameRenderingOn);
	ImGui::Checkbox("Albedo?", &m_isRenderAlbedoTextureOn);
	ImGui::Checkbox("Specular?", &m_isRenderSpecularTextureOn);
	ImGui::Checkbox("Normals?", &m_isRenderNormalTextureOn);
	ImGui::Checkbox("Ambient occlusion?", &m_isRenderAmbientOcclusionTextureOn);
	ImGui::End();

	ImGui::Begin("Model data.");
	ImGui::Text("Object triangle count:%d", triangleCount);
	ImGui::Text("Object vertex count:%d", vertexCount);
	ImGui::Text("Object face count:%d", faceCount);
	ImGui::Text("Texture count:%d", 6);
	ImGui::Text("File path %s", "Dummy path");

	ImGui::Separator();
	ImGui::Text("Scene settings.");
	ImGui::Checkbox("UseWorldGrid?", &m_isWireGridOn);
	ImGui::Checkbox("Enable lights?", &m_isRenderingLightOn);
	ImGui::Checkbox("Enable skybox?", &m_isSkyboxOn);
	ImGui::End();
	ImGui::Begin("Loading panel");
	ImGui::Text("Loading stuff here.");
	ImGui::End();

	ImGui::Begin("Loading panel");
	if (ImGui::Button("Open obj file"))
	{
		//Loading of object starts.
		char* path = OpenDialog();

		//Use path to load obj data;

		free(path);

	}
	ImGui::Text("Loading stuff here.");
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

nfdchar_t* OBJ_Viewer::RenderingSettings::OpenDialog()
{
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_OpenDialog("obj", NULL, &outPath);
	if (result == NFD_OKAY) {
		return outPath;
	}
	else {
		printf("Error: %s\n", NFD_GetError());
		
	}
	return nullptr;
}
