#pragma once
#include "gpu_side/VertexAttributeObject.h"
#include "Material.h"
#include "pch.h"

namespace OBJ_Viewer
{
	class Mesh {
	public:
		Mesh(std::unique_ptr<VertexAttributeObject> meshVAo, std::shared_ptr<Material> material = {});
		const VertexAttributeObject& GetMeshVAO()const { return *this->m_vao; }
		const Material& GetMaterial()const { return *m_Material; }
	private:
		std::shared_ptr<Material> m_Material;
		std::unique_ptr<VertexAttributeObject> m_vao;
	};
}

