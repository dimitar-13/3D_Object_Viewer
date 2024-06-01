#pragma once
#include"ShaderClass.h"
#include"ModelLoader.h"
#include"Scene/Camera.h"
#include"RenderingCoordinator.h"
namespace OBJ_Viewer {
	class Renderer
	{
	public:
		static void RenderMesh(const ShaderClass& shaderToUse, const Mesh& mesh, const Camera& mainCamera);
		static void RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, const Camera& mainCamera);
		static void IsWireFrameOn(bool isOn) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL - isOn); }
		static void BindMaterialTexture(const ShaderClass& shaderToUse, std::shared_ptr<Texture> textureToBind, GLenum textureUnit, const char* textureName);
		//void DrawSkybox();
	};
}
