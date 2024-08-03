#pragma once
#include "gpu_side/VertexAttributeObject.h"
#include "Material.h"
#include "pch.h"

namespace OBJ_Viewer
{
	class Mesh {
	public:
		Mesh(std::unique_ptr<VertexAttributeObject> meshVAo, size_t material_index = 0);
		const VertexAttributeObject& GetMeshVAO()const { return *this->m_vao; }
        size_t GetMaterialIndex()const { return m_meshRegistryMaterialIndex; }
	private:
        size_t m_meshRegistryMaterialIndex;
		std::unique_ptr<VertexAttributeObject> m_vao;
	};
}

