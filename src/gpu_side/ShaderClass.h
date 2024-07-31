#pragma once
#include "pch.h"
#include "gpu_side/UniformBuffer.h"
namespace OBJ_Viewer
{
	enum ShaderSourceType_
	{
		ShaderSourceType_kUknown,
		ShaderSourceType_kVertex = GL_VERTEX_SHADER,
		ShaderSourceType_kFragment = GL_FRAGMENT_SHADER,
		ShaderSourceType_kGeometry = GL_GEOMETRY_SHADER
	};
	class ShaderClass
	{
	public:
		//TODO:Shaders will be stored in a file and they can have different type like: skyboxShader,wireframe shader and etc. Every shader will start with #SHADER_TYPE:(the type of the shader).
		ShaderClass(const char* shader_file_path);
		void SetUniformSet2FloatVector(const char* name, const glm::vec2& value)const { glProgramUniform2fv(this->m_shaderHandle, FindUniform(name), 1, &value[0]); }
		void SetUniformSet3FloatVector(const char* name, const glm::vec3& value)const { glProgramUniform3fv(this->m_shaderHandle, FindUniform(name), 1, &value[0]); }
		void SetUniformSet4x4FloatMatrix(const char* name, const glm::mat4& value)const { glProgramUniformMatrix4fv(this->m_shaderHandle, FindUniform(name), 1,GL_FALSE, &value[0][0]); }
		void SetUniformSet3x3FloatMatrix(const char* name, const glm::mat3& value)const { glProgramUniformMatrix3fv(this->m_shaderHandle, FindUniform(name), 1, GL_FALSE, &value[0][0]); }
		void SetUniformSet1Int(const char* name, const int value)const { glProgramUniform1i(this->m_shaderHandle, FindUniform(name),value); }
		void SetUniformSet1Float(const char* name, const float value)const { glProgramUniform1f(this->m_shaderHandle, FindUniform(name), value); }
		void BindUniformBufferToShader(const UniformBuffer& uniform_buffer)const;
		std::vector<std::string> GetShaderUniformList()const;
		void UseShader()const { glUseProgram(this->m_shaderHandle); }
	private:
		std::unordered_map<ShaderSourceType_,std::string>ReadShaderSource(const char* path);
		std::string ReadIncludedShaderFIle(const std::string& absolute_path_to_included_shader_path);
		bool IsShaderCompilerSuccessfully(const GLuint shader_gpu_handle);
		bool IsProgramLinkedSuccessfully()const;
		GLuint CompileShader(const GLenum shader_type, const std::string& shader_source);
		GLint FindUniform(const char* name_of_uniform)const;
	private:
		int m_shaderHandle;
		mutable std::unordered_map<std::string, int> m_uniformHash;
	};
}

