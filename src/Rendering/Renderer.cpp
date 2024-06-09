#include "Renderer.h"
void OBJ_Viewer::Renderer::RenderMesh(const ShaderClass& shaderToUse, const Mesh& mesh, const Camera& mainCamera)
{
	shaderToUse.UseShader();
	shaderToUse.UniformSet4x4FloatMatrix("ViewProjMatrix", mainCamera.GetViewProjMatrix());
	mesh.GetMeshVAO().BindBuffer();
	glDrawElements(GL_TRIANGLES, mesh.GetMeshVAO().GetIndexCount(), GL_UNSIGNED_INT, NULL);
	mesh.GetMeshVAO().UnBind();	
}

void OBJ_Viewer::Renderer::RenderGrid(const ShaderClass& shaderToUse, const Mesh& mesh, const Camera& mainCamera, const GridData gridInfo)
{
	shaderToUse.UseShader();
	glm::mat4 view, proj;
	mainCamera.GetViewAndProjectionSeparate(&view, &proj);
	shaderToUse.UniformSet4x4FloatMatrix("ProjectionMatrix", proj);
	shaderToUse.UniformSet4x4FloatMatrix("ViewMatrix", view);
	shaderToUse.UniformSet1Float("gridInfo.gridScale", gridInfo.gridScale);
	shaderToUse.UniformSet3FloatVector("gridInfo.gridLineColor", gridInfo.gridLineColor);
	shaderToUse.UniformSet1Int("gridInfo.isAxisShaded", gridInfo.isAxisShaded);


	mesh.GetMeshVAO().BindBuffer();
	glDrawElements(GL_TRIANGLES, mesh.GetMeshVAO().GetIndexCount(), GL_UNSIGNED_INT, NULL);
	mesh.GetMeshVAO().UnBind();
}

void OBJ_Viewer::Renderer::RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, const Camera& mainCamera)
{
	/*Using early depth testing : Instead of rendering the skybox first we try to renderer it last.For this however we need a way to say that the skybox depth is less
	than the already rendered stuff to the screen so that it does not ovveride them.By making the skybox depth component w at the end we will get x/w, y/w and z wich is w so
	w/w resulting depth equal to 1.0f this is the hightest depth possible so using 'GL_LEQUAL' to make it so it will pass if its only less than the current value but it will
	never be less because is 1.0f so we get nothing where there are objects and where pixels are not mapped we get the skybox colors.*/
	glDepthFunc(GL_LEQUAL);
	skyboxShader.UseShader();
	skyboxShader.UniformSet4x4FloatMatrix("ViewProjMatrix", mainCamera.GetViewProjNoTranslation());
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