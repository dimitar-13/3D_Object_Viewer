#include "Mesh.h"
OBJ_Viewer::Mesh::Mesh(std::vector<OBJ_Viewer::Vertex> vertexData, std::vector<unsigned int>indexData, glm::mat4 transform, std::shared_ptr<Material> material)
	:m_ModelMatrix(transform), m_vao(vertexData, indexData)
{

	m_Material = material.get() != nullptr ? material : std::make_shared<Material>();
}
