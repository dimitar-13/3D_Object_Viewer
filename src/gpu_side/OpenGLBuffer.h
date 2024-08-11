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
		BufferType_ type_of_buffer = BufferType_kVertexBuffer;
		size_t bufferSize;
		GPUDataType_ dataType = GPUDataType_kUnsignedByte;
		BufferUsageType_ usageType = BufferUsageType_kStaticDraw;
		void* data = nullptr;
	};
    /**
     * @brief Class representing a GPU buffer for managing OpenGL memory.
     *
     * This class abstracts the management of GPU buffers used in OpenGL, such as those for index data,
     * vertex data, or pixel buffer objects (PBO). It provides methods to handle buffer operations, including
     * binding and unbinding the buffer to the OpenGL context.
     *
     * The `OpenGLBuffer` class is designed to simplify interactions with GPU memory by providing a higher-level
     * interface for managing buffer resources and their associated data.
     *
     * @note In the future I might make it so that all buffer classes are composition of this class but for now since we
     * are not repeating that much code this is fine. Also composition can introduce code duplication too and in this case the benefits are
     * not in favor of it since most functions are single OpenGL call.
     */
	class OpenGLBuffer
	{
	public:
        /**
         * @brief Constructs an `OpenGLBuffer` and initializes it with the provided data.
         *
         * This constructor creates an `OpenGLBuffer` object and initializes the buffer with the data specified in
         * the `BufferData` parameter. This data typically includes the buffer's contents, such as vertex data or index
         * data, which is uploaded to GPU memory upon initialization.
         *
         * @param data The data used to initialize the buffer. This includes the buffer's content and any additional
         *             parameters required for setup, such as size and usage hints.
         */
		OpenGLBuffer(const BufferData& data);
        OpenGLBuffer(const OpenGLBuffer& other) = delete; //Recommend for catching bugs if you don't intent to make copies.
        /**
         * @brief Binds the buffer to the OpenGL context.
         *
         * This method binds the buffer to the OpenGL context using the buffer type specified in the `BufferData`
         * provided during construction see BufferType_ for more info.
         * Binding the buffer makes it the current buffer for the specified target,
         * allowing OpenGL operations to affect this buffer.
         */
		void BindBuffer()const{ glBindBuffer(m_data.type_of_buffer, this->m_bufferHandle); }
        /**
         * @brief Unbinds the buffer to the OpenGL context.
         *
         * This method unbinds the buffer to the OpenGL context using the buffer type specified in the `BufferData`
         * provided during construction see BufferType_ for more info.
         */
		void UnbindBuffer()const { glBindBuffer(m_data.type_of_buffer,0); }
        /**
         * @brief Destroys the GPU-based buffer and releases associated resources.
         *
         * This destructor is responsible for cleaning up and releasing the GPU memory allocated for the buffer.
         * It ensures that the OpenGL buffer object is properly deleted and any associated resources are freed.
        */
		~OpenGLBuffer();
	private:
		GLuint m_bufferHandle;
		BufferData m_data;
	};
}

