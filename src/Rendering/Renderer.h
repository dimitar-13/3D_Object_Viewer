#pragma once
#include "gpu_side/ShaderClass.h"
#include "Scene/Camera.h"
#include "Scene/Skybox.h"
#include "Scene/Model.h"
#include "Core/Application.h"
#include "SceneConfigurationSettingsStruct.h"
namespace OBJ_Viewer {
	class Renderer
	{
	public:
		Renderer();
		void RenderMesh(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO);

        void RenderMeshWireframeLine(const ShaderClass& wireframe_shader, const VertexAttributeObject& mesh_VAO,
            const APP_SETTINGS::WireFrameSettings& wireframeAppSettings,const glm::mat3& viewport_matrix);

        void RenderMeshWireframePoint(const ShaderClass& wireframe_point_shader, const VertexAttributeObject& mesh_VAO,
            const APP_SETTINGS::WireFrameSettings& wireframeAppSettings, const glm::mat3& viewport_matrix);

        void RenderMeshSingleTexture(const ShaderClass& single_texture_shader,const Mesh& mesh_to_render,
            std::weak_ptr<Material> mesh_material, MaterialTextures_ material_texture_name);

        void RenderSolidColor(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO, const glm::vec3& color);

        void RenderMeshWithLight(const ShaderClass& light_shader, const VertexAttributeObject& meshVAO,
            std::weak_ptr<Material> mesh_material, MaterialRenderingFalgs_ materialFlags,
            APP_SETTINGS::SceneLightInfo lightInfo,const glm::vec3& camera_position);

        void RenderCheckboardPattern(const ShaderClass& checkerboard_shader, const VertexAttributeObject& meshVAO,
            const APP_SETTINGS::UV_ViewAppSetting& uv_settings);

		void RenderGrid(const ShaderClass& shaderToUse, const VertexAttributeObject& grid_VAO,const glm::vec3& camera_position,
            const APP_SETTINGS::GridData gridInfo);

		void RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, bool is_camera_perspective);

	private:
		std::unique_ptr<Texture> CreateDefaultTexture(const std::string& path);

        void BindMeshMaterialToShader(const ShaderClass& shaderToUse,
            const Material& material, MaterialRenderingFalgs_ renderMaterialFlags);

        void BindMaterialTextureToShader(const ShaderClass& shaderToUse,const Texture& textureToBind,
            GLenum textureUnit, const char* textureName);
	private:
		std::unique_ptr <Texture> m_defaultWhiteTexture;
		std::unique_ptr <Texture> m_defaultNormal;
	};
}
