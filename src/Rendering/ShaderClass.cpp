#include "ShaderClass.h"
#include<GL/glew.h>
#include<fstream>
#include<array>
#include<iostream>
#include<unordered_map>
OBJ_Viewer::ShaderClass::ShaderClass(const char* filePath)
{
    std::vector<std::string> sources = readShaderSource(filePath);
    std::vector<GLuint>shaderHandles(sources.size());
    for (size_t i = 0; i < sources.size(); i++)
    {
        switch (i)
        {
        case VERTEX_SHADER:
            shaderHandles[i] = compileShader(GL_VERTEX_SHADER, &sources[VERTEX_SHADER]);
            break;
        case FRAGMENT_SHADER:
            shaderHandles[i] = compileShader(GL_FRAGMENT_SHADER, &sources[FRAGMENT_SHADER]);
            break;
        default:
            break;
        } 
    }
    this->m_shaderHandle = glCreateProgram();
    for (const auto handle : shaderHandles)
    {
        glAttachShader(this->m_shaderHandle, handle);
    }
    glLinkProgram(this->m_shaderHandle);

    isProgramLinkedSuccessfully();
    //TODO:Think what to do after this leave it as is or destroy the object(i don't like this idea thought).
}

void OBJ_Viewer::ShaderClass::BindUBOToShader(const UniformBuffer& buffer)const
{
  const int bufferIndex = glGetUniformBlockIndex(m_shaderHandle, buffer.GetBufferName().c_str());
  glUniformBlockBinding(m_shaderHandle, bufferIndex, buffer.GetBindingPoint());
}

std::vector<std::string> OBJ_Viewer::ShaderClass::GetShaderUniformList()const
{
    int count; 
    glGetProgramiv(this->m_shaderHandle, GL_ACTIVE_UNIFORMS,&count);
    constexpr uint8_t buffSize = 50;
    if (!count)
        return std::vector<std::string>();

    std::vector<std::string> result(count);
    int Name_Size;
    GLenum uType;
    for (int i = 0; i < count; i++)
    {
        glGetActiveUniform(this->m_shaderHandle, i, buffSize, NULL, &Name_Size, &uType, result[i].data());
    }
    return result;
    
}

std::vector<std::string> OBJ_Viewer::ShaderClass::readShaderSource(const char* path)
{
    std::fstream sourceFile;
    sourceFile.open(path);
    if (!sourceFile.is_open())
    {
        std::cout << "Was not able to open file at path:" << path <<'\n';
    }
    std::string line;
    std::vector<std::string>shaderSources(2);
    std::string* p_currentShaderSource = NULL;
    while (std::getline(sourceFile, line))
    {
        if (line.compare("#Shader:vertex") == 0)
        {
            p_currentShaderSource = &shaderSources[VERTEX_SHADER];
            continue;
        }
        else if (line.compare("#Shader:fragment") == 0)
        {
            p_currentShaderSource = &shaderSources[FRAGMENT_SHADER];
            continue;
        }
        *p_currentShaderSource += line +'\n';
        //TODO:Add support for differed shader types like tessellation,geo,compute etc.
    }
    sourceFile.close();
    return shaderSources;
}

bool OBJ_Viewer::ShaderClass::isShaderCompilerSuccessfully(const GLuint Shader)
{
    GLint result;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        char infoLog[255];
        GLsizei length;
        glGetShaderInfoLog(Shader,sizeof(infoLog),&length,&infoLog[0]);
        std::cout << "[ERROR]:Shader failed to compile with error:\n" << infoLog << '\n';
        return false;
    }

    return true;
}

bool OBJ_Viewer::ShaderClass::isProgramLinkedSuccessfully()
{
    GLint result;
    glGetProgramiv(this->m_shaderHandle, GL_LINK_STATUS, &result);
    if (!result)
    {
        char infoLog[255];
        GLsizei length;
        glGetProgramInfoLog(this->m_shaderHandle, sizeof(infoLog), &length, &infoLog[0]);
        std::cout << "[ERROR]:Program failed to link with error:\n" << infoLog << '\n';
        return false;
    }
    return true;
}

GLuint OBJ_Viewer::ShaderClass::compileShader(const GLenum shaderType, const std::string* shaderSource)
{
    GLuint shaderHandle =  glCreateShader(shaderType);
    const char* source = shaderSource->c_str();
    //function expects array of strings so forced to this :)
    glShaderSource(shaderHandle, 1, &source, NULL);
    glCompileShader(shaderHandle);
    //Avoids branching since false is 0 and 0 is a non valid handle.
    return shaderHandle * isShaderCompilerSuccessfully(shaderHandle);
}

GLint OBJ_Viewer::ShaderClass::findUniform(const char* name)const
{
    if (this->m_uniformHash.find(name) != this->m_uniformHash.end())
        return this->m_uniformHash.at(name);  
    int UniformID = glGetUniformLocation(this->m_shaderHandle, name); 
    if (UniformID != -1)
    {
       this->m_uniformHash[name]  = UniformID;
       return this->m_uniformHash[name];
    }
    std::cout << "[WARNING]:Failed to find uniform with name: " << name << '\n';
}
