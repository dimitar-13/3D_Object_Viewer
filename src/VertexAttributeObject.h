#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<vector>
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
	//struct VertexSolidColor
	//{
	//	glm::vec3 position;
	//	glm::vec3 normal;
	//};
	class VertexAttributeObject
	{
	public:
		VertexAttributeObject(std::vector<Vertex> vertexData,std::vector<unsigned int>indexData);
		VertexAttributeObject(std::vector<glm::vec3> vertexData,std::vector<unsigned int>indexData);
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

