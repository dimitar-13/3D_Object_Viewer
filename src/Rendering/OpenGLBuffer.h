#pragma once
#include<GL/glew.h>
namespace OBJ_Viewer {
	enum BufferType
	{
		OPENGL_VERTEX_BUFFER = GL_ARRAY_BUFFER,
		OPENGL_INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
		OPENGL_PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
		OPENGL_PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER
	};
	//TODO:Texture has the same enum get into a global data type enum.
	enum BufferDataType
	{
		OPENGL_UNSIGNED_BYTE_DATA = GL_UNSIGNED_BYTE,
		OPENGL_UINT_DATA = GL_UNSIGNED_INT,
		OPENGL_FLOAT_DATA = GL_FLOAT
	};
	enum BufferUsageType
	{
		OPENGL_STATIC_DRAW_BUFFER_USAGE = GL_STATIC_DRAW,
		OPENGL_STREAM_DRAW_BUFFER_USAGE = GL_STREAM_DRAW,
		OPENGL_STREAM_READ_BUFFER_USAGE = GL_STREAM_READ,
		OPENGL_STEAM_COPY_BUFFER_USAGE = GL_STREAM_COPY,
		OPENGL_STATIC_READ_BUFFER_USAGE = GL_STATIC_READ,
		OPENGL_STATIC_COPY_BUFFER_USAGE = GL_STATIC_COPY,
		OPENGL_DYNAMIC_DRAW_BUFFER_USAGE = GL_DYNAMIC_DRAW,
		OPENGL_DYNAMIC_READ_BUFFER_USAGE = GL_DYNAMIC_READ,
		OPENGL_DYNAMIC_COPY_BUFFER_USAGE = GL_DYNAMIC_COPY
	};
	struct BufferData
	{
		BufferType type = OPENGL_VERTEX_BUFFER;
		size_t bufferSize;
		BufferDataType dataType = OPENGL_UNSIGNED_BYTE_DATA;
		BufferUsageType usageType = OPENGL_STATIC_DRAW_BUFFER_USAGE;
		void* data = nullptr;
	};

	class OpenGLBuffer
	{
	public:
		OpenGLBuffer(BufferData data);
		void BindBuffer()const{ glBindBuffer(m_data.type, this->m_bufferHandle); }
		void UnbindBuffer()const { glBindBuffer(m_data.type,0); }
		~OpenGLBuffer();
	private:
		GLuint m_bufferHandle;
		BufferData m_data;
	};
}

