#include "AppState.h"
#include"MeshGeneratingMethods.h"
OBJ_Viewer::AppState::AppState(Window* pAppWindow, InputHandler* pInputHandler, BaseMeshType typeOfDefaultMeshToLoad)
	:m_sceneFramebuffer(pAppWindow->GetWindowSize().m_winWidth, pAppWindow->GetWindowSize().m_winHeight, FRAMEBUFFER_COLOR_ATTACHMENT)
{
	switch (typeOfDefaultMeshToLoad)
	{
	case OBJ_Viewer::MESH_TYPE_CUBE:
		m_currentlyLoadedModel = std::shared_ptr<Model>(GenerateCubeModel());
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
	m_pInputHandler = pInputHandler;
}

void OBJ_Viewer::AppState::LoadModel(char* path)
{
	if (!path)
		return;

	ModelLoader loader;
	m_currentlyLoadedModel.reset(loader.LoadModel(path));
}

void OBJ_Viewer::AppState::LoadSkybox(std::vector<char*> paths)
{
	if (!paths.empty())
	{
		this->m_Skybox.reset(new Skybox(paths));
	}
}
void OBJ_Viewer::AppState::SwapSkyboxFaces(SkyboxFace toSwap, SkyboxFace with)
{
	this->m_Skybox->SwapSkyboxFaceTextures(toSwap,with);
}
//TODO:Change to resize Scene;
void OBJ_Viewer::AppState::ResizeBuffer(int newWidth, int newHeight)
{
	auto size = m_sceneFramebuffer.GetBufferSize();
	if(!(size.first == newWidth && size.second == newHeight))
		glViewport(0, 0, newWidth, newHeight);

	this->m_sceneFramebuffer.ResizeFramebuffer(newWidth, newHeight);
}
