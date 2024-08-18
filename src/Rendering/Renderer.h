#pragma once
#include "gpu_side/ShaderClass.h"
#include "Scene/Camera.h"
#include "Scene/Skybox.h"
#include "Scene/Model.h"
#include "Core/Application.h"
#include "SceneConfigurationSettingsStruct.h"
namespace OBJ_Viewer {
    /**
     * @brief Class housing the rendering functions used by the application.
     *
     * This class contains all of the rendering the function used by the application. The class knows how to set up a shader for a specific
     * type of rendering Studio light ,light calculation etc.
     * The class also creates 2 dummy textures used for material rendering.
     *  
     */
	class Renderer
	{
	public:
        /**
         * @brief Initialize the render class.
         *
         * Creates 2 dummy textures that will be used in case if a texture don't exist. Or is disabled.
         * This is done to avoid shader branching.
         * - Normal texture: A 1 blue pixel texture RGB(0,0,255).
         * - While texture : A 1 white pixel texture RGB(255,255,255).This is used for color,ambient occlusion and specular
         */
		Renderer();
        /**
         * @brief Renders a single mesh using a specified shader and vertex attribute object.
         *
         * This function binds the appropriate shader program and vertex attribute object (VAO) to render the mesh.
         * It assumes that the necessary OpenGL state and resources (like shaders and VAOs) are correctly set up before this call.
         *
         * @param shaderToUse The shader program to be used for rendering the mesh. It must be compiled and linked before use.
         * @param meshVAO The vertex attribute object that describes the mesh's vertex data layout and buffer bindings.  
         */
		void RenderMesh(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO);
        /**
         * @brief Sets up the necessary shader resources and renders the mesh in wireframe mode.
         * 
         * This function sets up the necessary shader resources required for rendering the mesh in wireframe mode and
         * then calls the `RenderMesh` function to perform the actual rendering. It takes care of setting up
         * any specific settings or transformations needed for the wireframe rendering.
         * 
         * @param wireframe_shader The shader program to be used for rendering the mesh in wireframe mode.
         * @param mesh_VAO The vertex attribute object that describes the mesh's vertex data layout and buffer bindings.
         * @param wireframeAppSettings Settings related to wireframe rendering, such as line thickness or color. These settings
         * are applied to the shader before rendering.
         * @param viewport_matrix A matrix used by the wireframe shader.
         */
        void RenderMeshWireframeLine(const ShaderClass& wireframe_shader, const VertexAttributeObject& mesh_VAO,
            const APP_SETTINGS::WireFrameSettings& wireframeAppSettings,const glm::mat4& viewport_matrix);
        /**
          * @brief Sets up the necessary shader resources and renders the mesh in wireframe point mode.
          *
          * This function sets up the necessary shader resources required for rendering the mesh in wireframe point mode and
          * then calls the `RenderMesh` function to perform the actual rendering. It takes care of setting up
          * any specific settings or transformations needed for the wireframe rendering.
          *
          * @param wireframe_point_shader The shader program to be used for rendering the mesh in wireframe mode.
          * @param mesh_VAO The vertex attribute object that describes the mesh's vertex data layout and buffer bindings.
          * @param wireframeAppSettings Settings related to wireframe rendering, such as line thickness or color. These settings
          * are applied to the shader before rendering.
          * @param viewport_matrix A matrix used by the wireframe shader.
          */
        void RenderMeshWireframePoint(const ShaderClass& wireframe_point_shader, const VertexAttributeObject& mesh_VAO,
            const APP_SETTINGS::WireFrameSettings& wireframeAppSettings, const glm::mat4& viewport_matrix);
        /**
         * @brief Renders a mesh using a single texture.
         *
         * This function renders a mesh with a single texture applied. The texture is specified by the `material_texture_name`
         * parameter, and it is used in conjunction with the `single_texture_shader` to render the mesh.
         *
         * The function checks the mesh material if the texture exist and retrieves it if it does. Then it binds it and 
         * renders the mesh.
         *
         * @param single_texture_shader The shader program to be used for rendering the mesh with a single texture. It must
         *                              be compiled and linked to handle texture mapping and rendering.
         * @param mesh_to_render The `Mesh` object representing the mesh to be rendered. It contains vertex attributes and other
         *                       necessary information for rendering.
         * @param mesh_material A `std::weak_ptr<Material>` pointing to the material used for the mesh. This material contains
         *                      the texture information needed for rendering.
         * @param material_texture_name The `MaterialTextures_` enum value specifying which texture from the material should be used
         * for rendering. This indicates which texture (e.g., albedo, normal) is applied to the mesh.
         */
        void RenderMeshSingleTexture(const ShaderClass& single_texture_shader,const Mesh& mesh_to_render,
            std::weak_ptr<Material> mesh_material, MaterialTextures_ material_texture_name);
        /**
         * @brief Renders a mesh using studio light calculations.
         * 
         * This function renders a mesh using an common 3D software effect. Not sure how is called I call it studio light.
         * The idea is that everything front of the camera is shaded with white or with the color that it is used while 
         * everything back is not shaded. This effect is used is mostly all of the rendering techniques that we do(meaning almost every
         * other function in this class uses this idea of rendering.).In the shader files there will be more information so see them.
         * 
         * 
         * @param shaderToUse The shader program to be used for rendering the mesh with a studio light.
         * @param meshVAO The mesh vertex attribute object used for rendering.
         * @param color The front shading color.
         */
        void RenderStudioWithLight(const ShaderClass& shaderToUse, const VertexAttributeObject& meshVAO, const glm::vec3& color);
        /**
        * @brief Renders a mesh using light calculation.
        *
        * This function renders a mesh using light calculation. There is currently 4 supported light models:
        * - Bling-phong light model.
        * - Toon shading light model.
        * - Rim light model.
        * - Toon + Rim light model.
        * The current model is choosen by the user and passed in the 'SceneLightInfo' struct along side the scene light infomation.
        * The light calculation is done with the mesh material. The user can select witch material textures are in the equation
        * using the 'MaterialRenderingFlags_'. If the user disables say the color texture the Renderer will replace it with a dummy texture.
        * 
        * @param light_shader The shader program to be used for rendering the mesh with lighting. It must be compiled and linked
        *                     to handle the selected light model and material properties.
        * @param meshVAO The `VertexAttributeObject` that contains the vertex attributes of the mesh to be rendered.
        * 
        * @param mesh_material A `std::weak_ptr<Material>` pointing to the material used for the mesh. This material contains
        *                   texture and property information required for light calculations. 
        *                   If the material is invalid, default or dummy values will be used.          
        * @param materialFlags The `MaterialRenderingFlags_` indicating which material textures are active and should be used in
        *                      the light calculation. Textures not enabled by these flags will be replaced with dummy textures.
        * @param lightInfo The `APP_SETTINGS::SceneLightInfo` struct containing information about the light sources and the chosen
        *                     light model. This includes settings for different types of light models and their parameters.
        * @param camera_position The position of the camera in world space, used to calculate lighting effects such as 
        *                          specular highlights.
        *
        */
        void RenderMeshWithLight(const ShaderClass& light_shader, const VertexAttributeObject& meshVAO,
            std::weak_ptr<Material> mesh_material, MaterialRenderingFlags_ materialFlags,
            APP_SETTINGS::SceneLightInfo lightInfo,const glm::vec3& camera_position);
        /**
         * @brief Renders a checkerboard pattern onto a mesh using the specified shader.
         * 
         * This function sets up the necessary shader resources required for rendering a checkerboard pattern.
         * The function takes a mesh and the checkboard shader and renders the pattern on it.The pattern is generated using the information
         * from uv_settings.
         * 
         * @param checkerboard_shader The shader program to be used for rendering the checkerboard pattern. It must be compiled
         * and linked to handle the checkerboard texture and its UV mapping.
         * @param meshVAO The `VertexAttributeObject` that contains the vertex attributes of the mesh on which the checkerboard
         * pattern will be rendered. It should be set up with the appropriate vertex attributes and buffer bindings.
         * @param uv_settings The `UV_ViewAppSetting` settings on how to generate the checkerboard pattern.
         */
        void RenderCheckboardPattern(const ShaderClass& checkerboard_shader, const VertexAttributeObject& meshVAO,
            const APP_SETTINGS::UV_ViewAppSetting& uv_settings);
        /**
         * @brief Renders a grid using the specified shader and vertex attribute object.
         * 
         * This function sets up the necessary shader resources required for rendering infinite grid. The mesh that the grid will 
         * be rendered must be a full screen normalized device coordinates(ndc) quad.
         * 
         * @param shaderToUse The shader program to be used for rendering the grid. It must be compiled and linked for
         * appropriate grid rendering.
         * @param grid_VAO It must be a full screen ndc quad.
         * @param camera_position The position of the camera in the 3D scene. This information may be used to adjust the
         * grid's appearance or position relative to the camera.
         * @param gridInfo Settings related to the grid rendering, such as grid size, color, or spacing. These settings
         * are applied to the shader before rendering to customize the appearance of the grid.
         *   
         */
		void RenderInfiniteGrid(const ShaderClass& shaderToUse, const VertexAttributeObject& grid_VAO,const glm::vec3& camera_position,
            const APP_SETTINGS::GridData gridInfo);
        /** 
         *@brief Renders the skybox using the specified shader and skybox object.
         *
         * This function sets up the necessary shader resources required for rendering the skybox.This should be called last of all renders
         * because it uses early depth testing to determine if the skybox should be Infront or not.
         *
         * @param skyboxShader The shader program to be used for rendering the skybox.It must be compiled and linked for
         * appropriate skybox rendering, typically including handling of the skybox's cube map texture.
         * @param skybox The `Skybox` object that contains the cubemap texture and other resources needed for rendering.
         * @param is_camera_perspective If the camera is perspective we use the projection. If orthographic we don't apply any projection
         * because the skybox is already in normalized device coordinates.
         */
		void RenderSkybox(const ShaderClass& skyboxShader, const Skybox& skybox, bool is_camera_perspective);
	private:
		std::unique_ptr<Texture> CreateDummyTexture(const std::string& path);
        /**
         * @brief Binds a material to the shader based on the provided MaterialRenderingFlags_. 
         * 
         * This function takes a material and based on if a texture is pressent and/or enabled binds the texture or a dummy texture.
         * The function uses 'BindMaterialTextureToShader()' to bind the texture.
         * 
         * @param shaderToUse Shader used to bind the material.
         * @param material The `Material` object containing the textures to be bound to the shader. The textures are accessed
         *                  based on the flags provided.
         * @param renderMaterialFlags The `MaterialRenderingFlags_` enum specifying which textures are enabled for rendering.
         * The flags determine which textures from the material will be bound and which will be replaced with dummy textures.
         */
        void BindMeshMaterialToShader(const ShaderClass& shaderToUse,
            const Material& material, MaterialRenderingFlags_ renderMaterialFlags);
        /**
         * @brief Binds a texture to a shader sampler.
         *
         * This function binds a texture to a specified texture unit in the shader program. It sets the appropriate texture unit
         * and assigns the texture to a shader sampler uniform. This setup allows the shader to access and use the texture during
         * rendering.
         *
         * @param shaderToUse Shader containing the sampler.
         * @param textureToBind The texture class that represents the texture.
         * @param textureUnit Texture unit 'GL_TEXTUREX'
         * @param textureName Name of the sampler uniform in the shader program.
         */
        void BindMaterialTextureToShader(const ShaderClass& shaderToUse,const Texture& textureToBind,
            GLenum textureUnit, const char* textureName);
	private:
		std::unique_ptr <Texture> m_defaultWhiteTexture; ///< 1x1 white RGB texture used as dummy texture for albedo,ambient occlusion and specular.
		std::unique_ptr <Texture> m_defaultNormal;      ///< 1x1 blue RGB texture used dummy texture for normal map.
	};
}
