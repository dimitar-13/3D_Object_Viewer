#pragma once
#include "pch.h"
#include "gpu_side/UniformBuffer.h"
namespace OBJ_Viewer
{
	enum ShaderSourceType_
	{
		ShaderSourceType_Uknown,
		ShaderSourceType_Vertex = GL_VERTEX_SHADER,
		ShaderSourceType_Fragment = GL_FRAGMENT_SHADER,
		ShaderSourceType_Geometry = GL_GEOMETRY_SHADER
	};
	class ShaderClass
	{
	public:
		//TODO:Shaders will be stored in a file and they can have different type like: skyboxShader,wireframe shader and etc. Every shader will start with #SHADER_TYPE:(the type of the shader).
		ShaderClass(const char* filePath);
		void UniformSet2FloatVector(const char* name, const glm::vec2& value)const { glProgramUniform2fv(this->m_shaderHandle, findUniform(name), 1, &value[0]); }
		void UniformSet3FloatVector(const char* name, const glm::vec3& value)const { glProgramUniform3fv(this->m_shaderHandle, findUniform(name), 1, &value[0]); }
		void UniformSet4x4FloatMatrix(const char* name, const glm::mat4& value)const { glProgramUniformMatrix4fv(this->m_shaderHandle, findUniform(name), 1,GL_FALSE, &value[0][0]); }
		void UniformSet3x3FloatMatrix(const char* name, const glm::mat3& value)const { glProgramUniformMatrix3fv(this->m_shaderHandle, findUniform(name), 1, GL_FALSE, &value[0][0]); }
		void UniformSet1Int(const char* name, const int value)const { glProgramUniform1i(this->m_shaderHandle, findUniform(name),value); }
		void UniformSet1Float(const char* name, const float value)const { glProgramUniform1f(this->m_shaderHandle, findUniform(name), value); }
		void BindUBOToShader(const UniformBuffer& buffer)const;
		std::vector<std::string> GetShaderUniformList()const;
		void UseShader()const { glUseProgram(this->m_shaderHandle); }
	private:
		std::unordered_map<ShaderSourceType_,std::string>readShaderSource(const char* path);
		std::string ReadIncludeFile(const std::string& path);
		bool isShaderCompilerSuccessfully(const GLuint Shader);
		bool isProgramLinkedSuccessfully()const;
		GLuint compileShader(const GLenum shaderType, const std::string& shaderSource);
		GLint findUniform(const char* name)const;
	private:
		int m_shaderHandle;
		mutable std::unordered_map<std::string, int> m_uniformHash;
	};
}

