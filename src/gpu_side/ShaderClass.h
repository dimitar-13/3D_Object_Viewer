#pragma once
#include "pch.h"
#include "gpu_side/UniformBuffer.h"
namespace OBJ_Viewer
{
    /**
     * @brief Class for managing and interacting with OpenGL shaders.
     *
     * The `ShaderClass` handles the creation, compilation, and linking of OpenGL shader programs. It provides methods for
     * setting uniform variables in the shaders and for managing shader resources. This class simplifies the use of shaders
     * by abstracting common tasks related to shader management.
     *
     * The class supports vertex, fragment, and geometry shaders, and reads shader source code from a specified file. The
     * source code file must contain shaders separated by special directives (`#Shader:<type>`), where `<type>` can be
     * `vertex`, `fragment`, or `geometry`. Additionally, shader files can include other shader files using `#include "path"`.
     *
     * Example shader file:
     * ```
     * An example shader can look like this:
     *
     * #Shader:vertex
     * #version 330 core
     * layout(location = 0) in vec3 position;
     *
     * void main()
     * {
     * 	gl_Position = vec4(position,1);
     * }
     * ```
     *
     * @note This class is designed to simplify the management of shader programs and their associated data. It provides
     * methods for setting various types of uniform variables and binding uniform buffers to the shader program.
     * 
     * @note The included file must not be with special directives (`#Shader:<type>`).
     * 
     */
	class ShaderClass
	{
    private:
        /**
         * @brief Enum used by the ShaderClass.
         *
         * This enum is used when reading from a single file. When we read a single shader file we extract at least 2 shaders vertex and fragment.
         * When we read the shaders we store the shader code in a hash map and using this enum we can access the each individual shader.
         * By also making it use the OpenGL macros we can make it type safe.
         */
        enum ShaderSourceType_
        {
            ShaderSourceType_kUknown,                           ///< Used for debugging.
            ShaderSourceType_kVertex = GL_VERTEX_SHADER,        ///< Represent the OpenGL macro 'GL_VERTEX_SHADER'.
            ShaderSourceType_kFragment = GL_FRAGMENT_SHADER,    ///< Represent the OpenGL macro 'GL_FRAGMENT_SHADER'.
            ShaderSourceType_kGeometry = GL_GEOMETRY_SHADER     ///< Represent the OpenGL macro 'GL_GEOMETRY_SHADER'.
        };
	public:
        /**
         * @brief Constructs a shader program from the provided shader file path.
         *
         * Reads the shader source code from the specified file path, compiles it, and links it into an OpenGL shader program.
         * The shader file can contain vertex, fragment, and geometry shaders. All the shaders must be in a single file and
         * be separated by `#Shader:<name>` directives.
         *
         * @param shader_file_path The file path to the shader source code.
         */
		ShaderClass(const char* shader_file_path);
        /**
         * @brief Sets a 2D vector uniform in the shader program.
         *
         * This method sets a 2D vector uniform variable in the shader program using the specified name and value.
         * If the uniform does not exist in debug it will log a message.
         *
         * @param name The name of the uniform variable in the shader.
         * @param value The 2D vector value to be assigned to the uniform.
         */
		void SetUniformSet2FloatVector(const char* name, const glm::vec2& value)const { glProgramUniform2fv(this->m_shaderHandle, FindUniform(name), 1, &value[0]); }
        /**
         * @brief Sets a 3D vector uniform in the shader program.
         *
         * This method sets a 3D vector uniform variable in the shader program using the specified name and value.
         * If the uniform does not exist in debug it will log a message.
         * 
         * @param name The name of the uniform variable in the shader.
         * @param value The 3D vector value to be assigned to the uniform.
         */
		void SetUniformSet3FloatVector(const char* name, const glm::vec3& value)const { glProgramUniform3fv(this->m_shaderHandle, FindUniform(name), 1, &value[0]); }
        /**
         * @brief Sets a 4x4 matrix uniform in the shader program.
         *
         * This method sets a 4x4 matrix uniform variable in the shader program using the specified name and value.
         * If the uniform does not exist in debug it will log a message.
         * 
         * @param name The name of the uniform variable in the shader.
         * @param value The 4x4 matrix value to be assigned to the uniform.
         */
        void SetUniformSet4x4FloatMatrix(const char* name, const glm::mat4& value)const { glProgramUniformMatrix4fv(this->m_shaderHandle, FindUniform(name), 1,GL_FALSE, &value[0][0]); }
        /**
         * @brief Sets a 3x3 matrix uniform in the shader program.
         *
         * This method sets a 3x3 matrix uniform variable in the shader program using the specified name and value.
         * If the uniform does not exist in debug it will log a message.
         * 
         * @param name The name of the uniform variable in the shader.
         * @param value The 3x3 matrix value to be assigned to the uniform.
         */
        void SetUniformSet3x3FloatMatrix(const char* name, const glm::mat3& value)const { glProgramUniformMatrix3fv(this->m_shaderHandle, FindUniform(name), 1, GL_FALSE, &value[0][0]); }
        /**
         * @brief Sets an integer uniform in the shader program.
         *
         * This method sets an integer uniform variable in the shader program using the specified name and value.
         * If the uniform does not exist in debug it will log a message.
         * Use this to set int,bool and sampler(2D,3D etc.) glsl data types. 
         * 
         * @param name The name of the uniform variable in the shader.
         * @param value The integer value to be assigned to the uniform.
         */
        void SetUniformSet1Int(const char* name, const int value)const { glProgramUniform1i(this->m_shaderHandle, FindUniform(name),value); }
        /**
         * @brief Sets a float uniform in the shader program.
         *
         * This method sets a float uniform variable in the shader program using the specified name and value.
         * If the uniform does not exist in debug it will log a message.
         * 
         * @param name The name of the uniform variable in the shader.
         * @param value The float value to be assigned to the uniform.
         */
        void SetUniformSet1Float(const char* name, const float value)const { glProgramUniform1f(this->m_shaderHandle, FindUniform(name), value); }
        /**
         * @brief Binds a uniform buffer to the shader program.
         *
         * This method binds a specified uniform buffer to the shader program by associating the buffer with
         * a uniform block within the shader. The method first retrieves the index of the uniform block by its name,
         * then binds the uniform buffer to that block using the buffer's binding point.
         *
         * @param uniform_buffer The `UniformBuffer` object to be bound to the shader program.
         *
         * @note This function assumes that the uniform buffer name matches the name of the uniform block in the shader code.
         */
        void BindUniformBufferToShader(const UniformBuffer& uniform_buffer)const;
		/**
         * Currently not used. 
         */
        std::vector<std::string> GetShaderUniformList()const;
        /**
         * @brief Binds the shader program for use in the OpenGL context.
         *
         * This method makes the shader program active, meaning that subsequent rendering operations will use
         * this shader program.
         */
        void UseShader()const { glUseProgram(this->m_shaderHandle); }
	private:
        /**
         * @brief Reads the shader source and constructs returns a map of shaders.
         * 
         * This method reads the shader source code from the specified file and separates the different shader stages
         * into a map, where each key corresponds to a `ShaderSourceType_` enum value. It handles shaders for different
         * stages such as vertex, fragment, and geometry, and organizes them accordingly.
         * 
         * The method processes `#include "path"` directives within the shader source code by calling `ReadIncludedShaderFile`
         * to include the content from the specified path. Note that if an included file is a complete shader stage (e.g., another
         * vertex shader or fragment shader), it will not be handled by this method. Therefore, the inclusion of complete shaders
         * within other shaders is not supported and will cause an error.
         * 
         * 
         * An example of the above statement.
         * #Shader:vertex
         * ...
         * ...
         * #include "AnotherVertexShader.glsl"
         * 
         * @param path The file path to the shader source code.
         * @return An unordered map where the key is a `ShaderSourceType_` enum value and the value is the shader source code
         *         as a string.
         */
		std::unordered_map<ShaderSourceType_,std::string>ReadShaderSource(const char* path);
        /**
         * @brief Reads the content of an included shader file.
         *
         * This method retrieves the data from a shader file specified by its absolute path. It reads the entire content of the file
         * and returns it as a string. This is typically used for handling `#include` directives within shader source code.
         * When using the #include the file that you are including don't need to have #Shader:<name of shader> and if it does this will cause
         * an error here.
         *
         * The method assumes that the path provided is absolute and valid. If the file cannot be read (e.g., due to incorrect path
         * or file access issues), the behavior of this method may vary based on implementation (e.g., it might throw an exception,
         * return an empty string, etc.).
         *
         * @param absolute_path_to_included_shader_path The absolute path to the shader file to be read.
         * @return A string containing the content of the shader file.
         *
         * @note Ensure that the provided path is correct and accessible. The method does not perform validation beyond attempting
         *       to read the file.
         * 
         */
		std::string ReadIncludedShaderFIle(const std::string& absolute_path_to_included_shader_path);
        /**
         * @brief Checks if the shader was successfully compiled.
         *
         * This method checks the compilation status of the shader associated with the given GPU handle. It queries the OpenGL
         * state for compilation success and retrieves the compile status. If the compilation failed, the method will also log
         * compilation errors.
         *
         * @param shader_gpu_handle The GPU handle (ID) of the shader to check.
         * @return `true` if the shader was compiled successfully; `false` otherwise.
         *
         * @note Ensure that the shader handle provided is valid and corresponds to a compiled shader.
         */
		bool IsShaderCompilerSuccessfully(const GLuint shader_gpu_handle);
        /**
         * @brief Checks if the shader program was successfully linked.
         *
         * This method checks the link status of the shader program associated with the current OpenGL context. It queries the OpenGL
         * state for linking success and retrieves the link status. If the linking failed, the method will log the linking errors.
         *
         * @return `true` if the shader program was linked successfully; `false` otherwise.
         *
         * @note Ensure that the shader program is correctly created and contains compiled shaders before checking the link status.
         */
		bool IsProgramLinkedSuccessfully()const;
        /**
         * @brief Creates and compiles a shader, returning the shader handle.
         *
         * This method creates and compiles a shader of the specified type from the provided shader source code. If the shader
         * compilation is successful, it returns a non-zero handle (ID) that represents the compiled shader. If the compilation
         * fails, it returns 0.
         *
         * The shader handle can be used for further operations such as linking the shader into a program or querying for compilation
         * status. Ensure to check the compilation status using `IsShaderCompilerSuccessfully` or similar methods to verify the success
         * of the shader compilation.
         *
         * @param shader_type The type of shader to be compiled (e.g., `GL_VERTEX_SHADER`, `GL_FRAGMENT_SHADER`).
         * @param shader_source The source code of the shader to be compiled.
         * @return A non-zero handle (ID) of the compiled shader if successful; 0 if the compilation fails.
         */
		GLuint CompileShader(const GLenum shader_type, const std::string& shader_source);
        /**
         * @brief Returns the index of the specified uniform in the current shader program.
         *
         * This method retrieves the index of a uniform variable in the shader program. It first checks if the uniform's index
         * is already cached in the `m_uniformHash` map. If it is not cached, the method queries the shader program for the uniform's
         * location using the provided name and updates the cache. If the uniform is not found in the shader, a warning message is logged.
         *
         * The uniform index can be used to set or query the value of the uniform variable within the shader program.
         *
         * @param name_of_uniform The name of the uniform variable whose index is to be retrieved.
         * @return The index of the uniform variable if found; otherwise, it returns -1 if the uniform does not exist.
         *
         * @note The index is cached in `m_uniformHash` for efficiency. If the uniform is not found, a warning is logged, and the
         *       method returns -1.
         */
		GLint FindUniform(const char* name_of_uniform)const;
	private:
		int m_shaderHandle;                                             ///< OpenGL shader handle/ID use this for shader related API calls.
		mutable std::unordered_map<std::string, int> m_uniformHash;     ///< Shader uniform hash for faster searches.
	};
}

