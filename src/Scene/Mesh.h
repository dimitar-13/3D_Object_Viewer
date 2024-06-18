#pragma once
#include "VertexAttributeObject.h"
#include"Material.h"
#include"glm/glm.hpp"
namespace OBJ_Viewer
{
	class Mesh {
	public:
		Mesh(std::unique_ptr<VertexAttributeObject> meshVAo, std::shared_ptr<Material> material = {});
		const VertexAttributeObject& GetMeshVAO()const { return *this->m_vao; }
		std::weak_ptr<Material> GetMaterial() { return m_Material; }
	private:
		std::shared_ptr<Material> m_Material;
		std::unique_ptr<VertexAttributeObject> m_vao;
	};
}

