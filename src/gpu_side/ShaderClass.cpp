#include "pch.h"
#include "ShaderClass.h"
#include"Logging/App_Logger.h"
OBJ_Viewer::ShaderClass::ShaderClass(const char* shader_file_path)
{
    std::string_view shader_path_string_view = shader_file_path;
    size_t string_last_slash_position = shader_path_string_view.find_last_of('/') + 1;
    m_ShaderName = 
        shader_path_string_view.substr(string_last_slash_position, shader_path_string_view.size() - string_last_slash_position);

    std::unordered_map<ShaderSourceType_, std::string> shader_sources = ReadShaderSource(shader_file_path);
    std::vector<GLuint> shader_handles {};
    for (const auto& item : shader_sources)
    {
        ShaderSourceType_ shader_key = item.first;
        const std::string& shader_source = item.second;

        shader_handles.push_back(CompileShader(shader_key, shader_source));
    }
    this->m_shaderHandle = glCreateProgram();
    for (const auto handle : shader_handles)
    {
        glAttachShader(this->m_shaderHandle, handle);
    }
    glLinkProgram(this->m_shaderHandle);

    IsProgramLinkedSuccessfully();
    //TODO:Think what to do after this leave it as is or destroy the object(i don't like this idea thought).
}

void OBJ_Viewer::ShaderClass::BindUniformBufferToShader(const UniformBuffer& uniform_buffer)const
{
  const int kUniformBufferIndex = glGetUniformBlockIndex(m_shaderHandle, uniform_buffer.GetBufferName().c_str());
  glUniformBlockBinding(m_shaderHandle, kUniformBufferIndex, uniform_buffer.GetBindingPoint());
}

std::vector<std::string> OBJ_Viewer::ShaderClass::GetShaderUniformList()const
{
    int uniform_list_count; 
    glGetProgramiv(this->m_shaderHandle, GL_ACTIVE_UNIFORMS,&uniform_list_count);
    constexpr uint8_t kMaxUniformArrayCount = 50;

    if (!uniform_list_count)
        return std::vector<std::string>();

    std::vector<std::string> result(uniform_list_count);
    int name_size;
    GLenum uniform_data_type;
    for (size_t i = 0; i < uniform_list_count; i++)
    {
        glGetActiveUniform(this->m_shaderHandle, i, kMaxUniformArrayCount, NULL, &name_size, &uniform_data_type, result[i].data());
    }
    return result;
    
}

std::unordered_map<OBJ_Viewer::ShaderClass::ShaderSourceType_,std::string> OBJ_Viewer::ShaderClass::ReadShaderSource(const char* path)
{
    std::fstream shader_source_file_stream;
    shader_source_file_stream.open(path);
    if (!shader_source_file_stream.is_open())
    {
        LOGGER_LOG_WARN("Was not able to open file at path:{0}", path);
    }
    std::string line;
    std::unordered_map<OBJ_Viewer::ShaderClass::ShaderSourceType_, std::string> shader_sources_result {};
    ShaderSourceType_ currently_reading_shader_key {};

    std::string shaderPathNoFilename = path;
    shaderPathNoFilename = shaderPathNoFilename.substr(0, shaderPathNoFilename.find_last_of('/') + 1);

    while (std::getline(shader_source_file_stream, line))
    {
        if (line.compare("#Shader:vertex") == 0)
        {
            currently_reading_shader_key = ShaderSourceType_kVertex;
            shader_sources_result[ShaderSourceType_kVertex] = std::string();
            continue;
        }
        else if (line.compare("#Shader:fragment") == 0)
        {
            currently_reading_shader_key = ShaderSourceType_kFragment;
            shader_sources_result[ShaderSourceType_kFragment] = std::string();
            continue;
        }
        else if (line.compare("#Shader:geometry") == 0)
        {
            currently_reading_shader_key = ShaderSourceType_kGeometry;
            shader_sources_result[ShaderSourceType_kGeometry] = std::string();
            continue;
        }
        else if (line.find("#include") != std::string::npos)
        {
            shader_sources_result[currently_reading_shader_key] += ReadIncludedShaderFIle(shaderPathNoFilename + line.substr(line.find_first_of('"') + 1, line.find_last_of('"') - line.find_first_of('"') - 1));
            continue;
        }
        shader_sources_result[currently_reading_shader_key] += line +'\n';
        //TODO:Add support for differed shader types like tessellation,geo,compute etc.
    }
    shader_source_file_stream.close();
    return shader_sources_result;
}

std::string OBJ_Viewer::ShaderClass::ReadIncludedShaderFIle(const std::string& absolute_path_to_included_shader_path)
{
    std::fstream include_file_stream;
    std::string line;
    std::string shader_file_source{};

    include_file_stream.open(absolute_path_to_included_shader_path);
    if (!include_file_stream.is_open())
    {
        LOGGER_LOG_WARN("Was not able to open file at path:{0}", absolute_path_to_included_shader_path);
        return shader_file_source;
    }

    while (std::getline(include_file_stream, line))
    {
        shader_file_source += line + '\n';
    }
    return shader_file_source;
}

bool OBJ_Viewer::ShaderClass::IsShaderCompilerSuccessfully(const GLuint shader_gpu_handle,ShaderSourceType_ shader_type)
{
    GLint shader_compilation_status;
    glGetShaderiv(shader_gpu_handle, GL_COMPILE_STATUS, &shader_compilation_status);
    if (!shader_compilation_status)
    {
        auto GetShaderTypeString = [](ShaderSourceType_ type_of_shader)
            {
                switch (type_of_shader)
                {
                case OBJ_Viewer::ShaderClass::ShaderSourceType_kVertex:
                    return "Vertex";
                case OBJ_Viewer::ShaderClass::ShaderSourceType_kFragment:
                    return "Fragment";
                case OBJ_Viewer::ShaderClass::ShaderSourceType_kGeometry:
                    return "Geometry";
                default:
                    return "Unknown";
                }
            };

        constexpr size_t kInfoLogMaxSize = 255;

        char info_log[kInfoLogMaxSize];
        GLsizei true_info_log_length {};
        glGetShaderInfoLog(shader_gpu_handle,sizeof(info_log),&true_info_log_length,&info_log[0]);
        LOGGER_LOG_ERROR(
            "Shader source compilation error.\n Shader name:'{0}'. \n Shader source type:'{1}'. \n Compilation error:{2}.",
           m_ShaderName,GetShaderTypeString(shader_type), info_log);
        return false;
    }

    return true;
}

bool OBJ_Viewer::ShaderClass::IsProgramLinkedSuccessfully()const
{
    GLint shader_program_link_status;
    glGetProgramiv(this->m_shaderHandle, GL_LINK_STATUS, &shader_program_link_status);

    if (!shader_program_link_status)
    {
        constexpr size_t kInfoLogMaxSize = 255;
        char error_info_log [kInfoLogMaxSize];
        GLsizei true_info_log_length;
        glGetProgramInfoLog(this->m_shaderHandle, sizeof(error_info_log), &true_info_log_length, &error_info_log[0]);
        LOGGER_LOG_ERROR(
            "Shader program linking error \n Shader name:'{0}'. \n Linking error:'{1}'.", m_ShaderName, error_info_log);

        return false;
    }
    return true;
}

GLuint OBJ_Viewer::ShaderClass::CompileShader(const GLenum shader_type, const std::string& shader_source)
{
    GLuint shader_gpu_handle =  glCreateShader(shader_type);
    const char* shader_opengl_const_cast = shader_source.c_str();
    //function expects array of strings so forced to this :)
    glShaderSource(shader_gpu_handle, 1, &shader_opengl_const_cast, NULL);
    glCompileShader(shader_gpu_handle);
    //Avoids branching since false is 0 and 0 is a non valid handle.
    return shader_gpu_handle * IsShaderCompilerSuccessfully(shader_gpu_handle,static_cast<ShaderSourceType_>(shader_type));
}

GLint OBJ_Viewer::ShaderClass::FindUniform(const char* name_of_uniform)const
{
    if (this->m_uniformHash.find(name_of_uniform) != this->m_uniformHash.end())
        return this->m_uniformHash.at(name_of_uniform);  
    int uniform_ID = glGetUniformLocation(this->m_shaderHandle, name_of_uniform); 
    if (uniform_ID != -1)
    {
       this->m_uniformHash[name_of_uniform]  = uniform_ID;
       return this->m_uniformHash[name_of_uniform];
    }
    LOGGER_LOG_WARN("Failed to find uniform with name '{0}'", name_of_uniform);
    return -1;
}
