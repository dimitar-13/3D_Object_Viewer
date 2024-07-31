#pragma once
#include "pch.h"
#include "gpu_side/ShaderClass.h"
namespace OBJ_Viewer {

	enum ShaderLibarryShaderName_
	{
		ShaderLibarryShaderName_kUknown,
		ShaderLibarryShaderName_kStudioLightShader,
		ShaderLibarryShaderName_kGridShader,
		ShaderLibarryShaderName_kSkyboxShader,
		ShaderLibarryShaderName_kLightShader,
		ShaderLibarryShaderName_kWireframeShader,
		ShaderLibarryShaderName_kPointShader,
		ShaderLibarryShaderName_kUVShader,
		ShaderLibarryShaderName_kSingleTextureShader,
		ShaderLibarryShaderName_kPostProcessShader,
		ShaderLibarryShaderName_kNormalShader,
	};

	class ShaderLibrary
	{
	public:
		ShaderLibrary();
		ShaderClass& GetShaderRef(ShaderLibarryShaderName_ nameOfShader) {assert(isShaderPresent(nameOfShader)); return m_shaderLibraryHash.at(nameOfShader); }
		ShaderClass* GetShaderPtr(ShaderLibarryShaderName_ nameOfShader) {assert(isShaderPresent(nameOfShader)); return &m_shaderLibraryHash.at(nameOfShader); }
		const std::unordered_map< ShaderLibarryShaderName_, ShaderClass>& GetShaderHash()const { return m_shaderLibraryHash; }
	private:
		bool isShaderPresent(ShaderLibarryShaderName_ nameOfShader) { return m_shaderLibraryHash.find(nameOfShader) != m_shaderLibraryHash.end(); }
	private:
		std::unordered_map< ShaderLibarryShaderName_, ShaderClass> m_shaderLibraryHash;
	};

}