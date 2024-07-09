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
		Renderer();
		void RenderMesh(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO, const Camera& mainCamera);
		void RenderMeshMaterialWithLight(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO,
			const Material& material, MaterialFlags renderMaterialFlags,const Camera& mainCamera);
		void RenderGrid(const ShaderClass& shaderToUse, const VertexAttributeObject& vao, const Camera& mainCamera,const GridData gridInfo);
		void RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, const Camera& mainCamera);
		void BindMaterialTexture(const ShaderClass& shaderToUse, std::shared_ptr<Texture> textureToBind, GLenum textureUnit, const char* textureName);
	private:
		std::shared_ptr<Texture> CreateDefaultTexture(const std::string& path);
	private:
		std::shared_ptr <Texture> m_defaultWhiteTexture;
		std::shared_ptr <Texture> m_defaultNormal;
	};
}
