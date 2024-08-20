#include "pch.h"
#include "UniformBuffer.h"

OBJ_Viewer::UniformBuffer::UniformBuffer(const std::string& buffer_name,GLuint buffer_binding_point,
	const uint32_t kBufferSizeInBytes, void* buffer_data)
{
	m_BufferName = buffer_name;
	m_bindingPoint = buffer_binding_point;
	glGenBuffers(1, &this->m_bufferHandle);
	this->BindBuffer();
		glBufferData(GL_UNIFORM_BUFFER, kBufferSizeInBytes, buffer_data, GL_STATIC_DRAW);
	this->UnbindBuffer();
}

OBJ_Viewer::UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &this->m_bufferHandle);
}


void OBJ_Viewer::UniformBuffer::SendBufferSubData(const size_t kSubDataOffset, const size_t kNewDataSizeInBytes,const void* buffer_data_to_send)
{
	this->BindBuffer();
	glBufferSubData(GL_UNIFORM_BUFFER, kSubDataOffset, kNewDataSizeInBytes, buffer_data_to_send);
	this->UnbindBuffer();
}
void OBJ_Viewer::UniformBuffer::BindBufferRange(const size_t kBufferDataOffset, const size_t kBufferSize) const
{
	this->BindBuffer();
		glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_bufferHandle, kBufferDataOffset, kBufferSize);
	this->UnbindBuffer();

}

