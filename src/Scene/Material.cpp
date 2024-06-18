#include "Material.h"
#include"Rendering/Renderer.h"

//This is creating before OpenGL can initialize.
//Yes this is not the best idea but for now it works.
static OBJ_Viewer::TextureStorage* defaultTextureStorage;
OBJ_Viewer::Material::Material(MaterialData data)
{
	if (defaultTextureStorage == nullptr)
		defaultTextureStorage = new TextureStorage();

	m_materialInfo = data;
	m_materialInfo.m_albedoTexture = m_materialInfo.m_albedoTexture != nullptr ? m_materialInfo.m_albedoTexture : defaultTextureStorage->Get1x1WhiteTexture();
	m_materialInfo.m_normalTexture = m_materialInfo.m_normalTexture != nullptr ? m_materialInfo.m_normalTexture : defaultTextureStorage->Get1x1BlueTexture();
	m_materialInfo.m_roughnessTexture = m_materialInfo.m_roughnessTexture != nullptr ? m_materialInfo.m_roughnessTexture : defaultTextureStorage->Get1x1WhiteTexture();
	m_materialInfo.m_ambientOcclusion = m_materialInfo.m_ambientOcclusion != nullptr ? m_materialInfo.m_ambientOcclusion : defaultTextureStorage->Get1x1WhiteTexture();

}

void OBJ_Viewer::Material::BindMaterialWithLightShader(const ShaderClass& shader, MaterialFlags flags)const
{
	shader.UseShader();
		if(flags & IS_ALBEDO_ON)
			Renderer::BindMaterialTexture(shader, m_materialInfo.m_albedoTexture, GL_TEXTURE1, "Mesh_material.albedo");
		else
			Renderer::BindMaterialTexture(shader, defaultTextureStorage->Get1x1WhiteTexture(), GL_TEXTURE1, "Mesh_material.albedo");
		//TODO:Change the lighting model.
		if (flags & IS_CUSTOM_SPECULAR_ON)
			Renderer::BindMaterialTexture(shader, m_materialInfo.m_roughnessTexture, GL_TEXTURE2, "Mesh_material.roughtness");
		else
			Renderer::BindMaterialTexture(shader, defaultTextureStorage->Get1x1WhiteTexture(), GL_TEXTURE2, "Mesh_material.roughtness");

		if (flags & IS_CUSTOM_NORMALS_ON)
			Renderer::BindMaterialTexture(shader, m_materialInfo.m_normalTexture, GL_TEXTURE3, "Mesh_material.normalMap");
		else
			Renderer::BindMaterialTexture(shader, defaultTextureStorage->Get1x1BlueTexture(), GL_TEXTURE3, "Mesh_material.normalMap");

		if (flags & IS_AMBIENT_OCCLUSION_ON)
			Renderer::BindMaterialTexture(shader, m_materialInfo.m_ambientOcclusion, GL_TEXTURE4, "Mesh_material.ambientOcclusion");
		else
			Renderer::BindMaterialTexture(shader, defaultTextureStorage->Get1x1WhiteTexture(), GL_TEXTURE4, "Mesh_material.ambientOcclusion");
		
	shader.UniformSet3FloatVector("Mesh_material.color", m_materialInfo.color);
	shader.UniformSet1Float("Mesh_material.specular", m_materialInfo.roughness);

}

void OBJ_Viewer::Material::BindMaterialWithShader(const ShaderClass& shader, MaterialFlags flags) const
{
	shader.UseShader();
	if (flags & IS_ALBEDO_ON)
		Renderer::BindMaterialTexture(shader, m_materialInfo.m_albedoTexture, GL_TEXTURE1, "Mesh_material.albedo");
	else
		Renderer::BindMaterialTexture(shader, defaultTextureStorage->Get1x1WhiteTexture(), GL_TEXTURE1, "Mesh_material.albedo");
	shader.UniformSet3FloatVector("Mesh_material.color", m_materialInfo.color);
}

OBJ_Viewer::TextureStorage::TextureStorage()
{
	m_1x1WhiteTexture = createTexture("D:/c++/OpenGl/3D_Object_Viewer/3D_Object_Viewer/Resources/WhiteTexture.png");
	m_1x1BlueTexture = createTexture("D:/c++/OpenGl/3D_Object_Viewer/3D_Object_Viewer/Resources/Normal_Map_dummy_texture.jpg");
}

std::shared_ptr<OBJ_Viewer::Texture> OBJ_Viewer::TextureStorage::createTexture(const char* path)
{
	TextureBuilder builder;
	int channelCount;
	TextureSize textureSize;
	TexturePixelDataWrapper reader(path, &textureSize, &channelCount);
	TextureFormat format = GetFormatByChannelCount(channelCount);
	return builder.SetTextureFormat(format).
		SetTextureInternalFormat(static_cast<TextureInternalFormat>(format)).SetTextureSize(textureSize).
		SetTexturePixelData(reader.GetTexturePixelData()).SetTextureWrapT(TEXTURE_WRAP_CLAMP_TO_EDGE).
		SetTextureWrapS(TEXTURE_WRAP_CLAMP_TO_EDGE).buildTexture();
}
