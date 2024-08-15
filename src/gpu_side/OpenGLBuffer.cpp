#include "pch.h"
#include "OpenGLBuffer.h"

OBJ_Viewer::OpenGLBuffer::OpenGLBuffer(const BufferData& data):
	m_bufferHandle(0),m_data(data)
{
	glGenBuffers(1, &this->m_bufferHandle);
	glBindBuffer(data.type_of_buffer, this->m_bufferHandle);
	glBufferData(data.type_of_buffer, data.bufferSize, data.data, data.usageType);
	glBindBuffer(data.type_of_buffer, 0);
}


OBJ_Viewer::OpenGLBuffer::~OpenGLBuffer()
{
	glDeleteBuffers(1, &this->m_bufferHandle);
}
