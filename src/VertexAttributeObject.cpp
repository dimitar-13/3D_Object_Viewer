#include "VertexAttributeObject.h"
#include<iostream>
OBJ_Viewer::VertexAttributeObject::VertexAttributeObject(std::vector<Vertex> vertexData, std::vector<unsigned int>indexData)
{
	glGenVertexArrays(1, &this->m_vertexAttributeObjHandle);
	glBindVertexArray(this->m_vertexAttributeObjHandle);

	glGenBuffers(1, &this->m_vertexBufferObjHandle);
	glGenBuffers(1, &this->m_indexBufferObjHandle);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexBufferObjHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexData.size(), vertexData.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_indexBufferObjHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)* indexData.size(), indexData.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvCoords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_indexCount = indexData.size();
	m_vertexCount = vertexData.size();
}

OBJ_Viewer::VertexAttributeObject::VertexAttributeObject(std::vector<glm::vec3> vertexData, std::vector<unsigned int>indexData)
{
	glGenVertexArrays(1, &this->m_vertexAttributeObjHandle);
	glBindVertexArray(this->m_vertexAttributeObjHandle);

	glGenBuffers(1, &this->m_vertexBufferObjHandle);
	glGenBuffers(1, &this->m_indexBufferObjHandle);


	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexBufferObjHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData[0]) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_indexBufferObjHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexData.size(), indexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, sizeof(float)*3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_indexCount = indexData.size();
	m_vertexCount = vertexData.size();
}
