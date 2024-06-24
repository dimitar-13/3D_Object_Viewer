#include "UniformBuffer.h"

OBJ_Viewer::UniformBuffer::UniformBuffer(std::string bufferName,GLuint bindPoint, const uint32_t buffSize,void* data)
{
	m_BufferName = bufferName;
	m_bindingPoint = bindPoint;
	glGenBuffers(1, &this->m_bufferHandle);
	this->BindBuffer();
		glBufferData(GL_UNIFORM_BUFFER, buffSize, data, GL_STATIC_DRAW);
	this->UnbindBuffer();
}

OBJ_Viewer::UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &this->m_bufferHandle);
}


void OBJ_Viewer::UniformBuffer::SendBufferSubData(const uint32_t dataBufferOffset, const uint32_t buffSize, const void* data)const
{
	this->BindBuffer();
	glBufferSubData(GL_UNIFORM_BUFFER, dataBufferOffset, buffSize, data);
	this->UnbindBuffer();
}
void OBJ_Viewer::UniformBuffer::BindBufferRange(const uint32_t dataBufferOffset, const uint32_t bufferRange) const
{
	this->BindBuffer();
		glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_bufferHandle, dataBufferOffset, bufferRange);
	this->UnbindBuffer();

}

