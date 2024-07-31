#include "pch.h"
#include "ShaderLibrary.h"
#include "ShaderPath.h"

constexpr size_t SHADER_COUNT = 9;
OBJ_Viewer::ShaderLibrary::ShaderLibrary()
{
	m_shaderLibraryHash.reserve(SHADER_COUNT);

	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kStudioLightShader,ShaderAssetHelper::GetConcatShaderPath("ClearColorMeshShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kGridShader,ShaderAssetHelper::GetConcatShaderPath("GridShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kSkyboxShader,ShaderAssetHelper::GetConcatShaderPath("SkyboxShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kLightShader,ShaderAssetHelper::GetConcatShaderPath("LightShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kWireframeShader,ShaderAssetHelper::GetConcatShaderPath("WireframeShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kPointShader,ShaderAssetHelper::GetConcatShaderPath("WireframePointShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kUVShader,ShaderAssetHelper::GetConcatShaderPath("UVShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kSingleTextureShader,ShaderAssetHelper::GetConcatShaderPath("SingleTextureInspectShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kPostProcessShader,ShaderAssetHelper::GetConcatShaderPath("PostProcessShader.glsl").c_str());
	m_shaderLibraryHash.emplace(ShaderLibarryShaderName_kNormalShader, ShaderAssetHelper::GetConcatShaderPath("NormalOrientationShader.glsl").c_str());

}
