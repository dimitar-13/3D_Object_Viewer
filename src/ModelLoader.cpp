#include "ModelLoader.h"

OBJ_Viewer::Model* OBJ_Viewer::ModelLoader::LoadModel(const char* path)
{
	//Its good if the caller is not in charge of freeing this memory.
	Model* result = new Model();
	//TODO:Implement
	return result;
}

OBJ_Viewer::Mesh::Mesh(std::vector<float> vertexData, std::vector<unsigned int> indexData, glm::mat4 transform):m_vao(vertexData, indexData),m_ModelMatrix(transform)
{

}
