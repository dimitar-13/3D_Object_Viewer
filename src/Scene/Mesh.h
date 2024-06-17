#pragma once
#include "VertexAttributeObject.h"
#include"Material.h"
#include"glm/glm.hpp"
namespace OBJ_Viewer
{
	class Mesh {
	public:
		Mesh(std::vector<OBJ_Viewer::Vertex> vertexData, std::vector<unsigned int>indexData, std::shared_ptr<Material> material = {});
		Mesh(std::vector<glm::vec3> vertexData, std::vector<unsigned int>indexData, std::shared_ptr<Material> material = {});

		const VertexAttributeObject& GetMeshVAO()const { return this->m_vao; }
		std::weak_ptr<Material> GetMaterial() { return m_Material; }
	private:
		std::shared_ptr<Material> m_Material;
		VertexAttributeObject m_vao;
	};
}

