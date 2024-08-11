#include "pch.h"
#include "VertexAttributeObject.h"

OBJ_Viewer::VertexAttributeObject::VertexAttributeObject(const std::vector<Vertex>& kVertexDataVectorRef,
	const std::vector<IndexDataType>& kIndexDataVectorRef)
{
	glGenVertexArrays(1, &this->m_vertexAttributeObjHandle);
	glBindVertexArray(this->m_vertexAttributeObjHandle);

	glGenBuffers(1, &this->m_vertexBufferObjHandle);
	glGenBuffers(1, &this->m_indexBufferObjHandle);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexBufferObjHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*kVertexDataVectorRef.size(), kVertexDataVectorRef.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_indexBufferObjHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(IndexDataType)* kIndexDataVectorRef.size(), kIndexDataVectorRef.data(), GL_STATIC_DRAW);
	
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

	m_vertexData.indexCount = kIndexDataVectorRef.size();
	m_vertexData.vertexCount = kVertexDataVectorRef.size();
}

OBJ_Viewer::VertexAttributeObject::VertexAttributeObject(const std::vector<glm::vec3>& kPostionDataVectorRef,
	const std::vector<IndexDataType>& kIndexDataVectorRef)
{
	glGenVertexArrays(1, &this->m_vertexAttributeObjHandle);
	glBindVertexArray(this->m_vertexAttributeObjHandle);

	glGenBuffers(1, &this->m_vertexBufferObjHandle);
	glGenBuffers(1, &this->m_indexBufferObjHandle);


	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexBufferObjHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(kPostionDataVectorRef[0]) * kPostionDataVectorRef.size(), kPostionDataVectorRef.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_indexBufferObjHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexDataType) * kIndexDataVectorRef.size(), kIndexDataVectorRef.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, sizeof(float)*3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_vertexData.indexCount = kIndexDataVectorRef.size();
	m_vertexData.vertexCount = kPostionDataVectorRef.size();
}

OBJ_Viewer::VertexAttributeObject::~VertexAttributeObject()
{
    glDeleteVertexArrays(0, &this->m_vertexAttributeObjHandle);
    glDeleteBuffers(0, &this->m_vertexBufferObjHandle);
    glDeleteBuffers(0, &this->m_indexBufferObjHandle);
}
