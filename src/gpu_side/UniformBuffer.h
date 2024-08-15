#pragma once
#include "pch.h"
namespace OBJ_Viewer {
    /**
     * @brief Class for managing an OpenGL Uniform Buffer Object (UBO).
     *
     * The `UniformBuffer` class provides an abstraction for handling Uniform Buffer Objects (UBOs) in OpenGL.
     * It allows for creating, binding, and updating UBOs, which are used to store uniform data that can be shared
     * across multiple shaders. This class also provides methods to bind the buffer to a specific binding point,
     * send sub-data to the buffer, and unbind the buffer when necessary.
    */
	class UniformBuffer
	{
	public:
        /**
         * @brief Constructs a Uniform Buffer Object (UBO) with the specified parameters.
         *
         * This constructor initializes a UBO with a given name, binding point, size, and initial data.
         * If no data is provided it will only allocate GPU memory.
         * The buffer is created and bound to the specified binding point in the OpenGL context.
         * 
         * @param buffer_name The name of the uniform buffer, useful for debugging and identification.
         * @param buffer_binding_point The binding point to which this uniform buffer will be bound in the shader.
         * @param kBufferSizeInBytes The size of the buffer in bytes.
         * @param buffer_data Pointer to the initial data to be stored in the buffer. Can be nullptr if no data is provided.
         */
		UniformBuffer(const std::string& buffer_name, GLuint buffer_binding_point,const uint32_t kBufferSizeInBytes, void* buffer_data);
        /**
         * @brief Destroys the Uniform Buffer Object and releases associated resources.
         *
         * This destructor cleans up the UBO by deleting the buffer from the GPU memory.
        */
		~UniformBuffer();
        /**
         * @brief Updates a portion of the uniform buffer with new data.
         *
         * This method allows for updating a specific range within the buffer. It is useful for partially updating
         * uniform data without the need to re-upload the entire buffer. Or to specify a data after buffer creation if 
         * the data in the constructor was null.
         *
         * @param kSubDataOffset The offset in bytes from the start of the buffer where the update begins.
         * @param kNewDataSizeInBytes The size of the new data to be written, in bytes.
         * @param buffer_data_to_send Pointer to the new data to be uploaded.
        */
		void SendBufferSubData(const size_t kSubDataOffset, const size_t kNewDataSizeInBytes, const void* buffer_data_to_send);
        /**
         * @brief Unbinds the currently bound Uniform Buffer Object.
         *
         * This method unbinds the UBO from the OpenGL context, effectively reverting to the default state where no UBO is bound.
        */
		void UnbindBuffer()const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }
        /**
         * @brief Binds the Uniform Buffer Object to the OpenGL context.
         *
         * This method binds the UBO, making it active for subsequent operations. Binding the buffer ensures that any
         * operations related to UBOs are directed to this buffer.
        */
		void BindBuffer()const { glBindBuffer(GL_UNIFORM_BUFFER, m_bufferHandle); }
        /**
         * @brief Binds a specific range of the uniform buffer to the binding point.
         *
         * This method binds a range of the UBO to a specific binding point, allowing for finer control over which
         * part of the buffer is accessible to shaders.
         *
         * @param kBufferDataOffset The offset in bytes from where the buffer range starts.
         * @param kBufferSize The size of the buffer range in bytes.
        */
		void BindBufferRange(const size_t kBufferDataOffset, const size_t kBufferSize)const;
        /**
         * @brief Returns the name of the uniform buffer.
         *
         * This method returns the name assigned to the UBO.
         *
         * @return The name of the uniform buffer.
        */
		const std::string& GetBufferName()const { return m_BufferName; }
        /**
         * @brief Returns the binding point of the uniform buffer.
         *
         * This method returns the binding point to which the UBO is bound in the OpenGL context.
         *
         * @return The binding point of the uniform buffer.
        */
		GLuint GetBindingPoint()const { return m_bindingPoint; }
	private:
		GLuint m_bufferHandle;     ///< The OpenGL buffer handle/ID use this for buffer related API calls.
		GLuint m_bindingPoint;     ///< Buffer binding point see the OpenGL docs for more info.
		std::string m_BufferName;  ///< The shader name of the buffer.
	};
}

