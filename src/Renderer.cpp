#include "Renderer.h"
#include "VertexAttributeObject.h"
#include"ShaderClass.h"
#include"Application.h"
#include"ModelLoader.h"
#include"MeshGeneratingMethods.h"
constexpr float scaleVal = 1 / 1;
//Test objs;
static std::vector<float> vertices = { -0.5f * scaleVal, -0.5f * scaleVal, // bottom left corner
										-0.5f * scaleVal,  0.5f * scaleVal, // top left corner
										 0.5f * scaleVal,  0.5f * scaleVal, // top right corner
										 0.5f * scaleVal, -0.5f * scaleVal, }; // bottom right corner

static std::vector <unsigned int> indices = { 0,1,2, // first triangle (bottom left - top left - top right)
					 0,2,3 }; // second triangle (bottom left - top right - bottom right)


void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	VertexAttributeObject vertexAttribObj = { vertices,indices };

	GLFWwindow* window = this->m_windowHandler->GetGLFW_Window();

	//Mesh square(vertices, indices,glm::mat4(0));
	//std::vector<Mesh> meshVec = { square };
	//Model defaultModel(meshVec);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
		

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_rendererShaders.colorShader.UseShader();
		m_rendererShaders.colorShader.UniformSet4x4FloatMatrix("ViewProjMatrix", m_Camera->GetViewProjMatrix());
		for (const auto& mesh : m_currentlyLoadedModel->GetModelMeshes())
		{
			mesh.GetMeshVAO().BindBuffer();
			glDrawElements(GL_TRIANGLES, mesh.GetMeshVAO().GetIndexCount(), GL_UNSIGNED_INT, NULL);
			mesh.GetMeshVAO().UnBind();
		}
		
		RenderScene();
		RenderImGui();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	//Set up the renderer based on the settings;

	/*if (this->m_rendererSettings.m_isSkyboxOn)
		this->m_mainRenderer.RenderObject(this->m_rendererShaders.skyboxShader);
	if(this->m_rendererSettings.m_isWireGridOn)
		this->m_mainRenderer.RenderObject();*/
	if (m_rendererSettings.m_isWireFrameRenderingOn)
		m_mainRenderer.EnableWireFrame();
	if (!m_rendererSettings.m_isWireFrameRenderingOn)
		m_mainRenderer.DisableWireFrame();
	//Submit to render;
	//m_mainRenderer.RenderObject(/*Shader to use*/, *m_currentlyLoadedModel);
}

OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Window* windowHandler)/*:m_Camera(})*/
{
	m_windowHandler = windowHandler;
	m_Camera =std::make_unique<Camera>(5.0f,m_windowHandler->GetWindowSize().m_winWidth,m_windowHandler->GetWindowSize().m_winHeight);
	m_windowHandler->GetMousePosNotifier().Attach(m_Camera.get());
	m_windowHandler->GetScrollChangeNotifier().Attach(m_Camera.get());
	m_windowHandler->GetWindowSizeChangeNotifier().Attach(m_Camera.get());
	m_currentlyLoadedModel.reset(GenerateCubeModel());
}

void OBJ_Viewer::RenderingCoordinator::RenderImGui()
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
	ImGui::Checkbox("Wireframe?", &m_rendererSettings.m_isWireFrameRenderingOn);
	ImGui::Checkbox("Albedo?", &m_rendererSettings.m_isRenderAlbedoTextureOn);
	ImGui::Checkbox("Specular?", &m_rendererSettings.m_isRenderSpecularTextureOn);
	ImGui::Checkbox("Normals?", &m_rendererSettings.m_isRenderNormalTextureOn);
	ImGui::Checkbox("Ambient occlusion?", &m_rendererSettings.m_isRenderAmbientOcclusionTextureOn);
	ImGui::End();

	ImGui::Begin("Model data.");
	ImGui::Text("Object triangle count:%d", triangleCount);
	ImGui::Text("Object vertex count:%d", vertexCount);
	ImGui::Text("Object face count:%d", faceCount);
	ImGui::Text("Texture count:%d", 6);
	ImGui::Text("File path %s", "Dummy path");

	ImGui::Separator();
	ImGui::Text("Scene settings.");
	ImGui::Checkbox("UseWorldGrid?", &m_rendererSettings.m_isWireGridOn);
	ImGui::Checkbox("Enable lights?", &m_rendererSettings.m_isRenderingLightOn);
	ImGui::Checkbox("Enable skybox?", &m_rendererSettings.m_isSkyboxOn);
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
		//this->m_currentlyLoadedModel = ModelLoader::LoadModel(path);
		free(path);
	}

	

	ImGui::Text("Loading stuff here.");
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

nfdchar_t* OBJ_Viewer::RenderingCoordinator::OpenDialog()
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

void OBJ_Viewer::Renderer::RenderObject(const ShaderClass& shaderToUse, const Model& modelToRender, const Camera& mainCamera)
{
	//TODO:Implement
}
