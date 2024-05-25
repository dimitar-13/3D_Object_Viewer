#include "Renderer.h"
void OBJ_Viewer::Renderer::RenderMesh(const ShaderClass& shaderToUse, const Mesh& mesh, const Camera& mainCamera)
{
	shaderToUse.UseShader();
	shaderToUse.UniformSet4x4FloatMatrix("ViewProjMatrix", mainCamera.GetViewProjMatrix());
	mesh.GetMeshVAO().BindBuffer();
	glDrawElements(GL_TRIANGLES, mesh.GetMeshVAO().GetIndexCount(), GL_UNSIGNED_INT, NULL);
	mesh.GetMeshVAO().UnBind();
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void OBJ_Viewer::Renderer::BindMaterialTexture(const ShaderClass& shaderToUse, std::shared_ptr<Texture> textureToBind, GLenum textureUnit, const char* textureName)
{
	shaderToUse.UseShader();
	glActiveTexture(textureUnit);
	textureToBind->BindTexture();
	shaderToUse.UniformSet1Int(textureName, textureUnit - GL_TEXTURE0);
}