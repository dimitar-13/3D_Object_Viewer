#include "pch.h"
#include "ShaderLibrary.h"
#include "ShaderPath.h"

constexpr size_t SHADER_COUNT = 9;
OBJ_Viewer::ShaderLibrary::ShaderLibrary()
{
	m_shaderLibraryHash.reserve(SHADER_COUNT);

	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_StudioLightShader,ShaderAssetHelper::GetConcatShaderPath("ClearColorMeshShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_GridShader,ShaderAssetHelper::GetConcatShaderPath("GridShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_SkyboxShader,ShaderAssetHelper::GetConcatShaderPath("SkyboxShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_LightShader,ShaderAssetHelper::GetConcatShaderPath("LightShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_WireframeShader,ShaderAssetHelper::GetConcatShaderPath("WireframeShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_PointShader,ShaderAssetHelper::GetConcatShaderPath("WireframePointShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_UVShader,ShaderAssetHelper::GetConcatShaderPath("UVShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_SingleTextureShader,ShaderAssetHelper::GetConcatShaderPath("SingleTextureInspectShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_PostProcessShader,ShaderAssetHelper::GetConcatShaderPath("PostProcessShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_NormalShader, ShaderAssetHelper::GetConcatShaderPath("NormalOrientationShader.glsl").c_str());

}
