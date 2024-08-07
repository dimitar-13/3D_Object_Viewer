#pragma once
#include <filesystem>
#include "Core/Config.h"
namespace OBJ_Viewer {
	
	class ShaderAssetHelper {
	public:
		static std::string GetConcatShaderPath(const char* shaderFileName)
		{
			return OBJ_VIEWER_SHADER_PATH + std::string(shaderFileName);
		}
	};
}