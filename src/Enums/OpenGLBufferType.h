#pragma once
#include <GL/glew.h>
namespace OBJ_Viewer {
    /**
     * @brief Enum representing different types of OpenGL buffers.
     *
     * This enum defines various buffer types used in OpenGL for different purposes. Each buffer type corresponds to a specific
     * OpenGL buffer object target and is used to store different types of data in GPU memory.
     *
     * - `BufferType_kVertexBuffer`: Represents a buffer used for storing vertex attribute data. It is bound to the `GL_ARRAY_BUFFER` target.
     * - `BufferType_kIndexBuffer`: Represents a buffer used for storing index data that defines the order of vertices in a mesh. It is bound to the `GL_ELEMENT_ARRAY_BUFFER` target.
     * - `BufferType_kPixelUnpackBuffer`: Represents a buffer used for pixel transfer operations from CPU memory to GPU memory. It is bound to the `GL_PIXEL_UNPACK_BUFFER` target.
     * - `BufferType_kPixelPackBuffer`: Represents a buffer used for pixel transfer operations from GPU memory to CPU memory. It is bound to the `GL_PIXEL_PACK_BUFFER` target.
     * - `BufferType_kUniformBuffer`: Represents a buffer used for storing uniform data that is shared between shaders. It is bound to the `GL_UNIFORM_BUFFER` target.
     *
     * Each buffer type is associated with an OpenGL buffer object target and is used to specify the kind of data stored in the buffer
     * and how it should be used in OpenGL operations.
     */
    enum BufferType_
    {
        BufferType_kVertexBuffer = GL_ARRAY_BUFFER,             ///< Buffer used for vertex attribute data
        BufferType_kIndexBuffer = GL_ELEMENT_ARRAY_BUFFER,      ///< Buffer used for index data
        BufferType_kPixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER, ///< Buffer used for pixel transfer from CPU to GPU
        BufferType_kPixelPackBuffer = GL_PIXEL_PACK_BUFFER,     ///< Buffer used for pixel transfer from GPU to CPU
        BufferType_kUniformBuffer = GL_UNIFORM_BUFFER           ///< Buffer used for uniform data shared between shaders
    };
}