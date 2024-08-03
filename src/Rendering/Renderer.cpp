#include "pch.h"
#include "Renderer.h"
#include "Helpers/TextureHelpers.h"
OBJ_Viewer::Renderer::Renderer()
{
	m_defaultWhiteTexture = CreateDefaultTexture("D:/c++/OpenGl/3D_Object_Viewer/3D_Object_Viewer/Resources/WhiteTexture.png");
	m_defaultNormal = CreateDefaultTexture("D:/c++/OpenGl/3D_Object_Viewer/3D_Object_Viewer/Resources/Normal_Map_dummy_texture.jpg");
}
void OBJ_Viewer::Renderer::RenderMesh(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO)
{
	shaderToUse.UseShader();

	meshVAO.BindBuffer();
	glDrawElements(GL_TRIANGLES, meshVAO.GetIndexCount(), GL_UNSIGNED_INT, NULL);
	meshVAO.UnBind();
}

void OBJ_Viewer::Renderer::RenderMeshWireframeLine(const ShaderClass& wireframe_shader,
    const VertexAttributeObject& mesh_VAO, const APP_SETTINGS::WireFrameSettings& wireframeAppSettings,
    const glm::mat3& viewport_matrix)
{    
    wireframe_shader.UseShader();
    wireframe_shader.SetUniformSet3FloatVector("u_frameColor", wireframeAppSettings.lineColor);
    wireframe_shader.SetUniformSet1Float("frameThickness", wireframeAppSettings.lineThickness);
    
    wireframe_shader.SetUniformSet3x3FloatMatrix("viewportMatrix", viewport_matrix);
    this->RenderMesh(wireframe_shader, mesh_VAO);
}

void OBJ_Viewer::Renderer::RenderMeshWireframePoint(const ShaderClass& wireframe_point_shader,
    const VertexAttributeObject& mesh_VAO,
   const APP_SETTINGS::WireFrameSettings& wireframeAppSettings, const glm::mat3& viewport_matrix)
{
    constexpr static float kPointSizeThicknessAdjustmentValue = 1.;
    wireframe_point_shader.UseShader();
    wireframe_point_shader.SetUniformSet3FloatVector("u_Color", wireframeAppSettings.lineColor);
    wireframe_point_shader.SetUniformSet3x3FloatMatrix("viewportMatrix", viewport_matrix);
    wireframe_point_shader.SetUniformSet1Float("pointSize", wireframeAppSettings.lineThickness + kPointSizeThicknessAdjustmentValue);
    this->RenderMesh(wireframe_point_shader, mesh_VAO);
}


void OBJ_Viewer::Renderer::RenderMeshSingleTexture(const ShaderClass& single_texture_shader, const Mesh& mesh_to_render,
    std::weak_ptr<Material> mesh_material, MaterialTextures_ material_texture_name)
{
    single_texture_shader.UseShader();
    if (auto material = mesh_material.lock())
    {
        if (auto texture = material->GetMaterialTexture(material_texture_name).lock())
            this->BindMaterialTextureToShader(single_texture_shader, *texture, GL_TEXTURE1, "textureToInspect");
        single_texture_shader.SetUniformSet1Int("uIsTextureInSRGB", material_texture_name == MaterialTextures_kAlbedo ? true : false);
    }
    this->RenderMesh(single_texture_shader, mesh_to_render.GetMeshVAO());
}


void OBJ_Viewer::Renderer::RenderSolidColor(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO, const glm::vec3& color)
{
    shaderToUse.UseShader();
    shaderToUse.SetUniformSet3FloatVector("u_Color", color);
    this->RenderMesh(shaderToUse, meshVAO);
}

void OBJ_Viewer::Renderer::RenderMeshWithLight(const ShaderClass& light_shader, const VertexAttributeObject& meshVAO,
    std::weak_ptr<Material> mesh_material, MaterialRenderingFalgs_ materialFlags,
    APP_SETTINGS::SceneLightInfo lightInfo,const glm::vec3& camera_position)
{
    light_shader.UseShader();

    light_shader.SetUniformSet1Int("isToonShadingOn",
        lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kToonShading
        || lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kRim_and_ToonShading);
    light_shader.SetUniformSet1Int("isRimLightOn",
        lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kRimShading ||
        lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kRim_and_ToonShading);

    light_shader.SetUniformSet3FloatVector("cameraPosition", camera_position);
    this->BindMeshMaterialToShader(light_shader, *mesh_material.lock(), materialFlags);

    this->RenderMesh(light_shader, meshVAO);

}

void OBJ_Viewer::Renderer::RenderCheckboardPattern(const ShaderClass& checkerboard_shader, const VertexAttributeObject& meshVAO,
    const APP_SETTINGS::UV_ViewAppSetting& uv_settings)
{
    checkerboard_shader.UseShader();
    checkerboard_shader.SetUniformSet1Float("uvScale", uv_settings.UV_scaleFactor);
    this->RenderMesh(checkerboard_shader, meshVAO);
}

void OBJ_Viewer::Renderer::BindMeshMaterialToShader(const ShaderClass& shaderToUse,
	const Material& material,MaterialRenderingFalgs_ renderMaterialFlags)
{
    bool is_albedo_texture_present_and_enabled =
        !material.GetMaterialTexture(MaterialTextures_kAlbedo).expired() && 
        renderMaterialFlags & OBJ_Viewer::MaterialRenderingFalgs_kAlbedoIsOn;

    bool is_normal_texture_present_and_enabled =
        !material.GetMaterialTexture(MaterialTextures_kNormal).expired() &&
        renderMaterialFlags & OBJ_Viewer::MaterialTextures_kNormal;

    bool is_metallic_texture_present_and_enabled =
        !material.GetMaterialTexture(MaterialTextures_kMetalic).expired() &&
        renderMaterialFlags & OBJ_Viewer::MaterialTextures_kMetalic;

    bool is_ambient_occlusion_texture_present_and_enabled =
        !material.GetMaterialTexture(MaterialTextures_kAmbientOcclusion).expired() &&
        renderMaterialFlags & OBJ_Viewer::MaterialTextures_kAmbientOcclusion;

	if (is_albedo_texture_present_and_enabled)
		BindMaterialTextureToShader(shaderToUse, *material.GetMaterialTexture(MaterialTextures_kAlbedo).lock(), GL_TEXTURE1, "Mesh_material.albedo");
	else
		BindMaterialTextureToShader(shaderToUse, *m_defaultWhiteTexture, GL_TEXTURE1, "Mesh_material.albedo");

	if (is_normal_texture_present_and_enabled)
		BindMaterialTextureToShader(shaderToUse, *material.GetMaterialTexture(MaterialTextures_kNormal).lock(), GL_TEXTURE2, "Mesh_material.normalMap");
	else
		BindMaterialTextureToShader(shaderToUse, *m_defaultWhiteTexture, GL_TEXTURE2, "Mesh_material.normalMap");

	if (is_metallic_texture_present_and_enabled)
		BindMaterialTextureToShader(shaderToUse, *material.GetMaterialTexture(MaterialTextures_kMetalic).lock(), GL_TEXTURE3, "Mesh_material.roughtness");
	else
		BindMaterialTextureToShader(shaderToUse, *m_defaultWhiteTexture, GL_TEXTURE3, "Mesh_material.roughtness");

	if (is_ambient_occlusion_texture_present_and_enabled)
		BindMaterialTextureToShader(shaderToUse,*material.GetMaterialTexture(MaterialTextures_kAmbientOcclusion).lock(), GL_TEXTURE4, "Mesh_material.ambientOcclusion");
	else
		BindMaterialTextureToShader(shaderToUse, *m_defaultWhiteTexture, GL_TEXTURE4, "Mesh_material.ambientOcclusion");

    shaderToUse.UseShader();
    shaderToUse.SetUniformSet3FloatVector("Mesh_material.color", material.GetMaterialBaseColor());
    shaderToUse.SetUniformSet1Float("Mesh_material.specular", material.GetMaterialRoughness());
}

void OBJ_Viewer::Renderer::RenderGrid(const ShaderClass& shaderToUse, const VertexAttributeObject& grid_VAO,
    const glm::vec3& camera_position,
    const APP_SETTINGS::GridData gridInfo)
{
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	shaderToUse.UseShader();
	shaderToUse.SetUniformSet3FloatVector("cameraPosition", camera_position);
	shaderToUse.SetUniformSet1Float("gridInfo.gridScale", gridInfo.gridScale);
	shaderToUse.SetUniformSet3FloatVector("gridInfo.gridLineColor", gridInfo.gridLineColor);
	shaderToUse.SetUniformSet1Int("gridInfo.isAxisShaded", gridInfo.isAxisShaded);

    RenderMesh(shaderToUse, grid_VAO);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

void OBJ_Viewer::Renderer::RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, bool is_camera_perspective)
{
	/*Using early depth testing : Instead of rendering the skybox first we try to renderer it last.For this however we need a way to say that the skybox depth is less
	than the already rendered stuff to the screen so that it does not override them.By making the skybox depth component w at the end we will get x/w, y/w and z wich is w so
	w/w resulting depth equal to 1.0f this is the highest depth possible so using 'GL_LEQUAL' to make it so it will pass if its only less than the current value but it will
	never be less because is 1.0f so we get nothing where there are objects and where pixels are not mapped we get the skybox colors.*/
    glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
    glEnable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	skyboxShader.UseShader();
	glActiveTexture(GL_TEXTURE0);
	skybox.BindSkyboxTexture();
	skyboxShader.SetUniformSet1Int("skyboxSampler", 0);
    skyboxShader.SetUniformSet1Int("uIsProjectionPerspective",is_camera_perspective);
    this->RenderMesh(skyboxShader,skybox.GetSkyboxVAO());

	glDepthFunc(GL_LESS);

    glDisable(GL_BLEND);

}
void OBJ_Viewer::Renderer::BindMaterialTextureToShader(const ShaderClass& shaderToUse,
    const Texture& textureToBind, GLenum textureUnit, const char* textureName)
{
	shaderToUse.UseShader();
	glActiveTexture(textureUnit);
	textureToBind.BindTexture();
	shaderToUse.SetUniformSet1Int(textureName, textureUnit - GL_TEXTURE0);
}

std::unique_ptr<OBJ_Viewer::Texture> OBJ_Viewer::Renderer::CreateDefaultTexture(const std::string& path)
{
	TexturePixelReader reader(path.c_str());
	TextureBuilder builder;
	TextureFormat_ format = reader.GetTextureFormat();

	return builder.SetTextureFormat(format).
		SetTextureInternalFormat(static_cast<TextureInternalFormat_>(format)).SetTextureSize(reader.GetTextureSize()).
		SetTexturePixelData(reader.GetTexturePixelData()).SetTextureWrapT(TextureWrap_kClampToEdge).
		SetTextureWrapS(TextureWrap_kClampToEdge).buildTexture();
}