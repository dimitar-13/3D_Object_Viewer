#pragma once
#include"ShaderClass.h"
#include"Scene/Camera.h"
#include"Scene/Skybox.h"
#include"Scene/Model.h"
#include"Application.h"
namespace OBJ_Viewer {
	enum RenderPrimitiveType
	{
		PRIMITIVE_TYPE_POINT = GL_POINT,
		PRIMITIVE_TYPE_LINE = GL_LINE,
		PRIMITIVE_TYPE_FILLED = GL_FILL,

	};

	class Renderer
	{
	public:
		static void RenderMesh(const ShaderClass& shaderToUse, const Mesh& mesh, const Camera& mainCamera);
		static void RenderMeshLight(const ShaderClass& shaderToUse, const Mesh& mesh, const Camera& mainCamera,const SceneLightInfo lightInfo);
		static void RenderGrid(const ShaderClass& shaderToUse, const VertexAttributeObject& vao, const Camera& mainCamera,const GridData gridInfo);
		static void RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, const Camera& mainCamera);
		static void RenderObjectWithWireFrame(const ShaderClass& shaderToUse, const Mesh& mesh, const Camera& mainCamera);
		static void SetRenderPrimitiveType(RenderPrimitiveType type = PRIMITIVE_TYPE_FILLED) { glPolygonMode(GL_FRONT_AND_BACK, type); }
		static void BindMaterialTexture(const ShaderClass& shaderToUse, std::shared_ptr<Texture> textureToBind, GLenum textureUnit, const char* textureName);
		//void DrawSkybox();
	};
}
