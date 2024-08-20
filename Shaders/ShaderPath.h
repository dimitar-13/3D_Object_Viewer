#pragma once
#include "Core/ProjectPathHelper.h"
namespace OBJ_Viewer {
	
	class ShaderAssetHelper {
	public:
		static std::string GetConcatShaderPath(const char* shaderFileName)
		{
			return ProjectPathHelper::GetCurrentExecutableFilePath().append("Shaders/") + std::string(shaderFileName);
		}
	};
}