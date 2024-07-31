#pragma once
#include "pch.h"
#include "Enums/GpuDataTypes.h"
namespace OBJ_Viewer {
	enum BufferType_
	{
		BufferType_kVertexBuffer = GL_ARRAY_BUFFER,
		BufferType_kIndexBuffer = GL_ELEMENT_ARRAY_BUFFER,
		BufferType_kPixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
		BufferType_kPixelPackBuffer = GL_PIXEL_PACK_BUFFER,
		BufferType_kUniformBuffer = GL_UNIFORM_BUFFER
	};
	//TODO:Texture has the same enum get into a global data type enum.

	enum BufferUsageType_
	{
		BufferUsageType_kStaticDraw = GL_STATIC_DRAW,
		BufferUsageType_kStreamDraw = GL_STREAM_DRAW,
		BufferUsageType_kStreamRead = GL_STREAM_READ,
		BufferUsageType_kStreamCopy = GL_STREAM_COPY,
		BufferUsageType_kStaticRead = GL_STATIC_READ,
		BufferUsageType_kStaticCopy = GL_STATIC_COPY,
		BufferUsageType_kDynamicDraw = GL_DYNAMIC_DRAW,
		BufferUsageType_kDynamicRead = GL_DYNAMIC_READ,
		BufferUsageType_kDynamicCopy = GL_DYNAMIC_COPY
	};
	struct BufferData
	{
		BufferType_ type = BufferType_kVertexBuffer;
		size_t bufferSize;
		GPUDataType_ dataType = GPUDataType_kUnsignedByte;
		BufferUsageType_ usageType = BufferUsageType_kStaticDraw;
		void* data = nullptr;
	};

	class OpenGLBuffer
	{
	public:
		OpenGLBuffer(const BufferData& data);
		void BindBuffer()const{ glBindBuffer(m_data.type, this->m_bufferHandle); }
		void UnbindBuffer()const { glBindBuffer(m_data.type,0); }
		~OpenGLBuffer();
	private:
		GLuint m_bufferHandle;
		BufferData m_data;
	};
}

