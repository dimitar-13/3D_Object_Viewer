#include "AppState.h"
#include"MeshGeneratingMethods.h"
#include"ShaderPath.h"
OBJ_Viewer::AppState::AppState(Window* pAppWindow, InputHandler& pInputHandler, BaseMeshType typeOfDefaultMeshToLoad)
	:m_sceneFramebuffer(pAppWindow->GetWindowSize().m_winWidth, pAppWindow->GetWindowSize().m_winHeight, FRAMEBUFFER_COLOR_ATTACHMENT)
	, m_pInputHandler(pInputHandler)
{

	std::shared_ptr<Mesh> mesh;
	ModelData data;
	switch (typeOfDefaultMeshToLoad)
	{
	case OBJ_Viewer::MESH_TYPE_CUBE:	
		m_currentlyLoadedModel = std::make_shared<Model>(std::vector<std::shared_ptr<Mesh>>{ std::move(GenerateCubeMesh())},glm::mat4(1),data);
		break;
	case OBJ_Viewer::MESH_TYPE_SPHERE:
		//
		break;
	case OBJ_Viewer::MESH_TYPE_TORUS:
		//
		break;
	default:
		break;
	}
	m_windowHandler = pAppWindow;
}


//TODO:Change to resize Scene;
void OBJ_Viewer::AppState::ResizeBuffer(int newWidth, int newHeight)
{
	auto size = m_sceneFramebuffer.GetBufferSize();
	if(!(size.first == newWidth && size.second == newHeight))
		glViewport(0, 0, newWidth, newHeight);

	this->m_sceneFramebuffer.ResizeFramebuffer(newWidth, newHeight);
}
