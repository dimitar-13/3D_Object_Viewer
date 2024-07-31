#include "pch.h"
#include "ShaderClass.h"
#include"Logging/App_Logger.h"
OBJ_Viewer::ShaderClass::ShaderClass(const char* filePath)
{
    std::unordered_map<ShaderSourceType_, std::string> sources = readShaderSource(filePath);
    std::vector<GLuint>shaderHandles {};
    for (const auto& item : sources)
    {
        ShaderSourceType_ shaderKey = item.first;
        const std::string& shaderSource = item.second;

        shaderHandles.push_back(compileShader(shaderKey,shaderSource));
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

std::unordered_map<OBJ_Viewer::ShaderSourceType_,std::string> OBJ_Viewer::ShaderClass::readShaderSource(const char* path)
{
    std::fstream sourceFile;
    sourceFile.open(path);
    if (!sourceFile.is_open())
    {
        LOGGER_LOG_WARN("Was not able to open file at path:{0}", path);
    }
    std::string line;
    std::unordered_map<OBJ_Viewer::ShaderSourceType_, std::string> result {};
    ShaderSourceType_ currentReadingShader {};

    std::string shaderPathNoFilename = path;
    shaderPathNoFilename = shaderPathNoFilename.substr(0, shaderPathNoFilename.find_last_of('\\') + 1);
    std::replace(shaderPathNoFilename.begin(), shaderPathNoFilename.end(), '\\', '/');

    while (std::getline(sourceFile, line))
    {
        if (line.compare("#Shader:vertex") == 0)
        {
            currentReadingShader = ShaderSourceType_Vertex;
            result[ShaderSourceType_Vertex] = std::string();
            continue;
        }
        else if (line.compare("#Shader:fragment") == 0)
        {
            currentReadingShader = ShaderSourceType_Fragment;
            result[ShaderSourceType_Fragment] = std::string();
            continue;
        }
        else if (line.compare("#Shader:geometry") == 0)
        {
            currentReadingShader = ShaderSourceType_Geometry;
            result[ShaderSourceType_Geometry] = std::string();
            continue;
        }
        else if (line.find("#include") != std::string::npos)
        {
            result[currentReadingShader] += ReadIncludeFile(shaderPathNoFilename + line.substr(line.find_first_of('"') + 1, line.find_last_of('"') - line.find_first_of('"') - 1));
            continue;
        }
        result[currentReadingShader] += line +'\n';
        //TODO:Add support for differed shader types like tessellation,geo,compute etc.
    }
    sourceFile.close();
    return result;
}

std::string OBJ_Viewer::ShaderClass::ReadIncludeFile(const std::string& path)
{
    std::fstream sourceFile;
    std::string line;
    std::string result{};

    sourceFile.open(path);
    if (!sourceFile.is_open())
    {
        LOGGER_LOG_WARN("Was not able to open file at path:{0}", path);
        return result;
    }

    while (std::getline(sourceFile, line))
    {
        result += line + '\n';
    }
    return result;
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
        LOGGER_LOG_ERROR("Shader failed to compile with error:{0}", infoLog);
        return false;
    }

    return true;
}

bool OBJ_Viewer::ShaderClass::isProgramLinkedSuccessfully()const
{
    GLint result;
    glGetProgramiv(this->m_shaderHandle, GL_LINK_STATUS, &result);
    if (!result)
    {
        char infoLog[255];
        GLsizei length;
        glGetProgramInfoLog(this->m_shaderHandle, sizeof(infoLog), &length, &infoLog[0]);
        LOGGER_LOG_ERROR("Program failed to link with error:{0}", infoLog);
        return false;
    }
    return true;
}

GLuint OBJ_Viewer::ShaderClass::compileShader(const GLenum shaderType, const std::string& shaderSource)
{
    GLuint shaderHandle =  glCreateShader(shaderType);
    const char* source = shaderSource.c_str();
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
    LOGGER_LOG_WARN("Failed to find uniform with name '{0}'", name);
}
