#pragma once
#include "pch.h"
#include "Enums/GpuDataTypes.h"
#include "Enums/OpenGLBufferType.h"
namespace OBJ_Viewer {
 
    /**
     * @brief Enum representing buffer usage types for OpenGL buffer objects.
     *
     * This enum defines various usage patterns for OpenGL buffers, specifying how the buffer data will be used and updated
     * by the GPU. These usage types help optimize performance by informing the GPU about the intended usage of the buffer.
     *
     * - `BufferUsageType_kStaticDraw`: Indicates that the buffer will be used for static data that is rarely changed. It is optimized
     *   for drawing operations where the data is set once and used multiple times.
     * - `BufferUsageType_kStreamDraw`: Indicates that the buffer will be used for data that changes frequently and is used for drawing operations.
     * - `BufferUsageType_kStreamRead`: Indicates that the buffer will be used for data that changes frequently and will be read back by the CPU.
     * - `BufferUsageType_kStreamCopy`: Indicates that the buffer will be used for data that changes frequently and needs to be copied to another buffer.
     * - `BufferUsageType_kStaticRead`: Indicates that the buffer will be used for static data that is rarely changed and will be read back by the CPU.
     * - `BufferUsageType_kStaticCopy`: Indicates that the buffer will be used for static data that is rarely changed and needs to be copied to another buffer.
     * - `BufferUsageType_kDynamicDraw`: Indicates that the buffer will be used for data that changes frequently and is used for drawing operations.
     * - `BufferUsageType_kDynamicRead`: Indicates that the buffer will be used for data that changes frequently and will be read back by the CPU.
     * - `BufferUsageType_kDynamicCopy`: Indicates that the buffer will be used for data that changes frequently and needs to be copied to another buffer.
     *
     */
	enum BufferUsageType_
	{
		BufferUsageType_kStaticDraw = GL_STATIC_DRAW,   ///< Static data used for drawing operations
        BufferUsageType_kStreamDraw = GL_STREAM_DRAW,   ///< Data that changes frequently and is used for drawing operations
        BufferUsageType_kStreamRead = GL_STREAM_READ,   ///< Data that changes frequently and is read back by the CPU
        BufferUsageType_kStreamCopy = GL_STREAM_COPY,   ///< Data that changes frequently and needs to be copied to another buffer
        BufferUsageType_kStaticRead = GL_STATIC_READ,   ///< Static data used for reading back by the CPU
        BufferUsageType_kStaticCopy = GL_STATIC_COPY,   ///< Static data that needs to be copied to another buffer
        BufferUsageType_kDynamicDraw = GL_DYNAMIC_DRAW, ///< Data that changes frequently and is used for drawing operations
        BufferUsageType_kDynamicRead = GL_DYNAMIC_READ, ///< Data that changes frequently and is read back by the CPU
        BufferUsageType_kDynamicCopy = GL_DYNAMIC_COPY  ///< Data that changes frequently and needs to be copied to another buffer
	};

    /**
     * @brief Structure representing data for a GPU buffer.
     *
     * This structure contains the necessary information for defining and configuring a GPU buffer object in OpenGL. It includes details
     * about the buffer type, size, data format, usage pattern, and the actual data to be stored in the buffer.
     *
     * - `type_of_buffer`: Specifies the type of buffer being described. This determines the target to which the buffer will be bound
     *   (e.g., vertex buffer, index buffer, etc.). Default is `BufferType_kVertexBuffer`.
     * - `bufferSize`: Represents the size of the buffer in bytes. It indicates how much memory will be allocated for the buffer.
     * - `dataType`: Defines the data type of the buffer's contents, such as unsigned byte, unsigned int, or float. Default is `GPUDataType_kUnsignedByte`.
     * - `usageType`: Indicates how the buffer will be used by OpenGL, which influences its storage and performance characteristics.
     *   Options include static, dynamic, or stream usage patterns. Default is `BufferUsageType_kStaticDraw`.
     * - `data`: A pointer to the actual data to be stored in the buffer. This can be `nullptr` if no initial data is provided.
     *
     * This structure is used in the 'OpenGLBuffer' class to create a buffer.
     */
	struct BufferData
	{
		BufferType_ type_of_buffer = BufferType_kVertexBuffer;     ///< Type of buffer.See 'BufferType_' enum for more information.
		size_t bufferSize;                                         ///< Size of the buffer in bytes
		GPUDataType_ dataType = GPUDataType_kUnsignedByte;         ///< Data type of the buffer's contents. See 'GPUDataType_' for more information.
		BufferUsageType_ usageType = BufferUsageType_kStaticDraw;  ///< Usage pattern of the buffer. See 'BufferUsageType_' for more information.
		void* data = nullptr;                                      ///< Pointer to the buffer data; can be nullptr if no initial data is provided
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
     * 
     * @note The data pointer in the BufferData is not managed by this class and will not be freed by it. It is simply copied to the GPU memory.
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
		GLuint m_bufferHandle; ///< OpenGL buffer handle/ID use this for buffer related API calls.
		BufferData m_data;     ///< Buffer information data structure.
	};
}

