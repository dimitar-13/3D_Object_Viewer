#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<unordered_map>
#include<string>
namespace OBJ_Viewer
{
	class ShaderClass
	{
	public:
		//TODO:Shaders will be stored in a file and they can have different type like: skyboxShader,wireframe shader and etc. Every shader will start with #SHADER_TYPE:(the type of the shader).
		ShaderClass(const char* shaderType);
		void UniformSet3FloatVector(const char * name,const glm::vec3 value)const;
		void UniformSet4x4FloatMatrix(const char* name, const glm::mat4 value)const;
		void UniformSet1Int(const char* name, const int value)const;
	private:
		bool isShaderCompilerSuccessfully(const int Shader);
		bool isProgramCompilerSuccessfully();
		int compileShader(const GLenum shaderType,const char* shaderSource);
		int findUniform(const char* name);
	private:
		int m_shaderHandle;
		std::unordered_map<std::string, int> m_uniformHash;
	};
}

