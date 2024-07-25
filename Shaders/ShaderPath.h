#pragma once
#include <filesystem>
namespace OBJ_Viewer {
	
	class ShaderAssetHelper {
	public:
		static std::string& GetShaderPath()
		{
			if (s_path.empty())
				s_path = std::filesystem::current_path().string() + "\\Shaders";
			return s_path;
		}
		static std::string GetConcatShaderPath(const char* shaderFileName)
		{
			return GetShaderPath() + '\\' + std::string(shaderFileName);
		}
	private:
		inline static std::string s_path;
	};
}