#pragma once
#include "pch.h"
#include "gpu_side/ShaderClass.h"
namespace OBJ_Viewer {

	enum ShaderLibarryShaderName_
	{
		ShaderLibarryShaderName_Uknown,
		ShaderLibarryShaderName_StudioLightShader,
		ShaderLibarryShaderName_GridShader,
		ShaderLibarryShaderName_SkyboxShader,
		ShaderLibarryShaderName_LightShader,
		ShaderLibarryShaderName_WireframeShader,
		ShaderLibarryShaderName_PointShader,
		ShaderLibarryShaderName_UVShader,
		ShaderLibarryShaderName_SingleTextureShader,
		ShaderLibarryShaderName_PostProcessShader,
		ShaderLibarryShaderName_NormalShader,
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