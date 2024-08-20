#include "pch.h"
#include "Mesh.h"

OBJ_Viewer::Mesh::Mesh(std::unique_ptr<VertexAttributeObject> meshVAo, size_t material_index)
	: m_MeshVertexAttributeObject(std::move(meshVAo)), m_meshRegistryMaterialIndex(material_index)
{
}
