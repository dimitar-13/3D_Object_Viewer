#include "pch.h"
#include "Mesh.h"

OBJ_Viewer::Mesh::Mesh(std::unique_ptr<VertexAttributeObject> meshVAo, std::shared_ptr<Material> material)
	: m_vao(std::move(meshVAo)), m_Material( material)
{
}
