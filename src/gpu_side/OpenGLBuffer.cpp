#include "pch.h"
#include "OpenGLBuffer.h"

OBJ_Viewer::OpenGLBuffer::OpenGLBuffer(BufferData data):
	m_bufferHandle(0),m_data(data)
{
	glGenBuffers(1, &this->m_bufferHandle);
	glBindBuffer(data.type, this->m_bufferHandle);
	glBufferData(data.type, data.bufferSize, data.data, data.usageType);
	glBindBuffer(data.type, 0);
}

OBJ_Viewer::OpenGLBuffer::~OpenGLBuffer()
{
	glDeleteBuffers(1, &this->m_bufferHandle);
}
