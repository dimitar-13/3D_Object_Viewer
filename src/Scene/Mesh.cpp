#include "Mesh.h"
OBJ_Viewer::Mesh::Mesh(std::vector<OBJ_Viewer::Vertex> vertexData, std::vector<unsigned int>indexData, std::shared_ptr<Material> material)
	: m_vao(vertexData, indexData)
{

	m_Material = material.get() != nullptr ? material : std::make_shared<Material>();
}

OBJ_Viewer::Mesh::Mesh(std::vector<glm::vec3> vertexData, std::vector<unsigned int>indexData, std::shared_ptr<Material> material):
	m_vao(vertexData, indexData)
{
	m_Material = material.get() != nullptr ? material : std::make_shared<Material>();

}
