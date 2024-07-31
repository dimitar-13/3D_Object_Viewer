#pragma once
#include "pch.h"
namespace OBJ_Viewer
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uvCoords;
		glm::vec3 normal;
		glm::vec3 tangent;
		//TODO:Add more
	};
	struct VertexAttribData
	{
		size_t indexCount;
		size_t vertexCount;
	};

	class VertexAttributeObject
	{
	public:
		using IndexDataType = unsigned int;

		VertexAttributeObject(const std::vector<Vertex>& kVertexDataVectorRef,const std::vector<IndexDataType>& kIndexDataVectorRef);
		VertexAttributeObject(const std::vector<glm::vec3>& kPostionDataVectorRef, const std::vector<IndexDataType>& kIndexDataVectorRef);
		size_t GetIndexCount()const { return this->m_vertexData.indexCount; }
		size_t GetVertexCount()const { return this->m_vertexData.vertexCount; }
		void BindBuffer()const{ glBindVertexArray(this->m_vertexAttributeObjHandle); }
		void UnBind()const { glBindVertexArray(0); }
	private:
		GLuint m_vertexBufferObjHandle;
		GLuint m_vertexAttributeObjHandle;
		GLuint m_indexBufferObjHandle;
		VertexAttribData m_vertexData;
	};
}

