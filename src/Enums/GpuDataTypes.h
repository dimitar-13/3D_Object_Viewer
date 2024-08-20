#pragma once
#include <GL/glew.h>
namespace OBJ_Viewer {

    /**
     * @brief Enum representing GPU-based buffer data type.
     *
     * Each enum value corresponds to a specific OpenGL data type, allowing for type-safe specification of data formats when working with GPU buffers.
     *
     * - `GPUDataType_kUnsignedByte`: Represents the `GL_UNSIGNED_BYTE` type, typically used for 8-bit unsigned integer data.
     * - `GPUDataType_kUnsignedInt`: Represents the `GL_UNSIGNED_INT` type, typically used for 32-bit unsigned integer data.
     * - `GPUDataType_kFloat`: Represents the `GL_FLOAT` type, used for floating-point data.
     */
	enum GPUDataType_
	{
		GPUDataType_kUnsignedByte = GL_UNSIGNED_BYTE, ///< Internally have value of 'GL_UNSIGNED_BYTE' and it represents unsigned char
		GPUDataType_kUnsignedInt = GL_UNSIGNED_INT, ///< Internally have value of 'GL_UNSIGNED_INT' and it represents 32-bit unsigned integer.
		GPUDataType_kFLoat = GL_FLOAT ///< Internally have value of 'GL_FLOAT' and it represents floating point.
	};
}