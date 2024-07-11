#include "Renderer.h"
OBJ_Viewer::Renderer::Renderer()
{
	m_defaultWhiteTexture = CreateDefaultTexture("D:/c++/OpenGl/3D_Object_Viewer/3D_Object_Viewer/Resources/WhiteTexture.png");
	m_defaultNormal = CreateDefaultTexture("D:/c++/OpenGl/3D_Object_Viewer/3D_Object_Viewer/Resources/Normal_Map_dummy_texture.jpg");
}
void OBJ_Viewer::Renderer::RenderMesh(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO, const Camera& mainCamera)
{
	shaderToUse.UseShader();

	meshVAO.BindBuffer();
	glDrawElements(GL_TRIANGLES, meshVAO.GetIndexCount(), GL_UNSIGNED_INT, NULL);
	meshVAO.UnBind();
}

void OBJ_Viewer::Renderer::RenderMeshMaterialWithLight(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO,
	const Material& material,MaterialFlags renderMaterialFlags, const Camera& mainCamera)
{
	shaderToUse.UseShader();

	meshVAO.BindBuffer();

	if (!material.GetMaterialTexture(MATERIAL_TEXTURE_ALBEDO).expired() && renderMaterialFlags & OBJ_Viewer::IS_ALBEDO_ON )
	{
		BindMaterialTexture(shaderToUse, material.GetMaterialTexture(MATERIAL_TEXTURE_ALBEDO).lock(), GL_TEXTURE1, "Mesh_material.albedo");
	}
	else
	{
		BindMaterialTexture(shaderToUse, m_defaultWhiteTexture, GL_TEXTURE1, "Mesh_material.albedo");
	}

	if (!material.GetMaterialTexture(MATERIAL_TEXTURE_NORMAL).expired() && renderMaterialFlags & OBJ_Viewer::IS_CUSTOM_NORMALS_ON)
	{
		BindMaterialTexture(shaderToUse, material.GetMaterialTexture(MATERIAL_TEXTURE_NORMAL).lock(), GL_TEXTURE2, "Mesh_material.normalMap");
	}
	else
	{
		BindMaterialTexture(shaderToUse, m_defaultWhiteTexture, GL_TEXTURE2, "Mesh_material.normalMap");
	}

	if (!material.GetMaterialTexture(MATERIAL_TEXTURE_ROUGHNESS_METALLIC).expired() && renderMaterialFlags & OBJ_Viewer::IS_CUSTOM_SPECULAR_ON)
	{
		BindMaterialTexture(shaderToUse, material.GetMaterialTexture(MATERIAL_TEXTURE_ROUGHNESS_METALLIC).lock(), GL_TEXTURE3, "Mesh_material.roughtness");
	}
	else
	{
		BindMaterialTexture(shaderToUse, m_defaultWhiteTexture, GL_TEXTURE3, "Mesh_material.roughtness");
	}

	if (!material.GetMaterialTexture(MATERIAL_TEXTURE_AMBIENT_OCCLUSION).expired() && renderMaterialFlags & OBJ_Viewer::IS_AMBIENT_OCCLUSION_ON)
	{
		BindMaterialTexture(shaderToUse,material.GetMaterialTexture(MATERIAL_TEXTURE_AMBIENT_OCCLUSION).lock(), GL_TEXTURE4, "Mesh_material.ambientOcclusion");
	}
	else
	{
		BindMaterialTexture(shaderToUse, m_defaultWhiteTexture, GL_TEXTURE4, "Mesh_material.ambientOcclusion");
	}

	shaderToUse.UniformSet3FloatVector("Mesh_material.color", material.GetMaterialData().color);
	shaderToUse.UniformSet1Float("Mesh_material.specular", material.GetMaterialData().roughness);
	glDrawElements(GL_TRIANGLES, meshVAO.GetIndexCount(), GL_UNSIGNED_INT, NULL);
	meshVAO.UnBind();
}

void OBJ_Viewer::Renderer::RenderGrid(const ShaderClass& shaderToUse, const VertexAttributeObject& vao, const Camera& mainCamera, const GridData gridInfo)
{
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	shaderToUse.UseShader();
	glm::mat4 view, proj;
	mainCamera.GetViewAndProjectionSeparate(&view, &proj);

	shaderToUse.UniformSet1Float("gridInfo.gridScale", gridInfo.gridScale);
	shaderToUse.UniformSet3FloatVector("gridInfo.gridLineColor", gridInfo.gridLineColor);
	shaderToUse.UniformSet1Int("gridInfo.isAxisShaded", gridInfo.isAxisShaded);

	vao.BindBuffer();
	glDrawElements(GL_TRIANGLES, vao.GetIndexCount(), GL_UNSIGNED_INT, NULL);
	vao.UnBind();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

void OBJ_Viewer::Renderer::RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, const Camera& mainCamera)
{
	/*Using early depth testing : Instead of rendering the skybox first we try to renderer it last.For this however we need a way to say that the skybox depth is less
	than the already rendered stuff to the screen so that it does not ovveride them.By making the skybox depth component w at the end we will get x/w, y/w and z wich is w so
	w/w resulting depth equal to 1.0f this is the hightest depth possible so using 'GL_LEQUAL' to make it so it will pass if its only less than the current value but it will
	never be less because is 1.0f so we get nothing where there are objects and where pixels are not mapped we get the skybox colors.*/
	glDepthFunc(GL_LEQUAL);
	skyboxShader.UseShader();
	glActiveTexture(GL_TEXTURE0);
	skybox.BindSkyboxTexture();
	skyboxShader.UniformSet1Int("skyboxSampler", 0);
	skybox.GetSkyboxVAO().BindBuffer();
	glDrawElements(GL_TRIANGLES, skybox.GetSkyboxVAO().GetIndexCount(), GL_UNSIGNED_INT, NULL);
	skybox.GetSkyboxVAO().UnBind();
	glDepthFunc(GL_LESS);

}
void OBJ_Viewer::Renderer::BindMaterialTexture(const ShaderClass& shaderToUse, std::shared_ptr<Texture> textureToBind, GLenum textureUnit, const char* textureName)
{
	shaderToUse.UseShader();
	glActiveTexture(textureUnit);
	textureToBind->BindTexture();
	shaderToUse.UniformSet1Int(textureName, textureUnit - GL_TEXTURE0);
}

std::shared_ptr<OBJ_Viewer::Texture> OBJ_Viewer::Renderer::CreateDefaultTexture(const std::string& path)
{
	TextureBuilder builder;
	int channelCount;
	TextureSize textureSize;
	TexturePixelDataWrapper reader(path.c_str(), &textureSize, &channelCount);
	TextureFormat format = GetFormatByChannelCount(channelCount);
	return builder.SetTextureFormat(format).
		SetTextureInternalFormat(static_cast<TextureInternalFormat>(format)).SetTextureSize(textureSize).
		SetTexturePixelData(reader.GetTexturePixelData()).SetTextureWrapT(TEXTURE_WRAP_CLAMP_TO_EDGE).
		SetTextureWrapS(TEXTURE_WRAP_CLAMP_TO_EDGE).buildTexture();
}