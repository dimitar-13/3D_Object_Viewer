#pragma once
#include "VertexAttributeObject.h"
#include"glm/glm.hpp"
namespace OBJ_Viewer
{
	struct Mesh
	{
		VertexAttributeObject m_vao;
		glm::mat4 m_transformMatrix;
	};
}

