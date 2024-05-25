#pragma once
#include "VertexAttributeObject.h"
#include"Material.h"
#include"glm/glm.hpp"
namespace OBJ_Viewer
{
	class Mesh {
	public:
		Mesh(std::vector<OBJ_Viewer::Vertex> vertexData, std::vector<unsigned int>indexData, glm::mat4 transform, std::shared_ptr<Material> material = {});
		const VertexAttributeObject& GetMeshVAO()const { return this->m_vao; }
		std::shared_ptr<Material> GetMaterial() { return m_Material; }
		const glm::mat4& GetModelMatrix()const { return this->m_ModelMatrix; }
	private:
		glm::mat4 m_ModelMatrix;
		std::shared_ptr<Material> m_Material;
		VertexAttributeObject m_vao;
	};
}

