#pragma once
#include <filesystem>
namespace OBJ_Viewer {
	inline std::string path;
	inline std::string& GetShaderPath()
	{
		if (path.empty())
			path = std::filesystem::current_path().string() + "\\Shaders";
		return path;
	}
	inline std::string GetConcatShaderPath(const char* shaderFileName)
	{
		return GetShaderPath() +'\\' + std::string(shaderFileName);
	}
}