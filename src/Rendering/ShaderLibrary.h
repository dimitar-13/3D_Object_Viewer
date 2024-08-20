#pragma once
#include "pch.h"
#include "gpu_side/ShaderClass.h"
namespace OBJ_Viewer {

    /**
     * @brief Enum used to get shader from the 'ShaderLibrary' class.
     * 
     * - ShaderLibraryShaderName_kUnknown:                  For debugging.
     * - ShaderLibraryShaderName_kStudioLightShader:        Shader representing studio light effect.  
     * - ShaderLibraryShaderName_kGridShader:               Shader for rendering the grid.
     * - ShaderLibraryShaderName_kSkyboxShader:             Shader for rendering the skybox.   
     * - ShaderLibraryShaderName_kLightShader:              Shader for rendering mesh with light calculations.
     * - ShaderLibraryShaderName_kWireframeShader:          Shader for rendering with wireframe(lines) on top of the mesh.
     * - ShaderLibraryShaderName_kPointShader:              Shader for rendering with wireframe(points) on top of the mesh.
     * - ShaderLibraryShaderName_kUVShader:                 Shader for rendering checkerboard pattern for UV inspection.
     * - ShaderLibraryShaderName_kSingleTextureShader:      Shader for rendering a single texture.
     * - ShaderLibraryShaderName_kPostProcessShader:        Shader for post-processing the final image.
     * - ShaderLibraryShaderName_kNormalShader:             Shader for displaying normal direction.
     */
	enum ShaderLibraryShaderName_
	{
		ShaderLibraryShaderName_kUnknown, ///< For debugging.
        /**
         * The shader from this enum will render the object with studio light only.
         *
         * Studio light is like popular 3D modelling software blender way of shading when in modeling mode. Where
         * front faces/pixels are lit up and the one back are darker.
         */
		ShaderLibraryShaderName_kStudioLightShader,
        /**
         * The shader from this enum renders a 3D infinite grid onto a full screen quad.
         *
         * The shader from this enum should be used on a full screen quad because its designed for it.
         */
		ShaderLibraryShaderName_kGridShader,
        /**
         * The shader from this enum will render cubemap texture on a normalize device coordinate(NDC) cube.
         * 
         * This shader is designed for a NDC cube.
         */
		ShaderLibraryShaderName_kSkyboxShader,
        /**
         * The shader from this enum will render a mesh using light calculations.
         * 
         * The shader works in linier space so it transforms the texture that are not in linear(albedo/color) and proceeds to 
         * do the light calculations. 
         * The shader have 4 different calculation modes:
         *  - Blin-Phong light shading model.
         *  - Toon light shading model.
         *  - Rim light shading model
         *  - Toon + Rim light shading model.
         * They are toggled by a uniform.
         * 
         * All of the outputted pixel colors from this shader are in linier space.
         */
		ShaderLibraryShaderName_kLightShader,
        /**
         * The shader from this enum renders a solid wireframe(with lines) on top of a mesh.
         *
         * The shader is from the nvidia white papers:
         * https://developer.download.nvidia.com/SDK/10/direct3d/Source/SolidWireframe/Doc/SolidWireframe.pdf
         */
		ShaderLibraryShaderName_kWireframeShader,
        /**
         * The shader from this enum renders a solid wireframe(with points) on top of a mesh.
         *
         * This is modified version of nvidia white papers wireframe shader but for rendering points.
         * 
         * nvidia white papers:https://developer.download.nvidia.com/SDK/10/direct3d/Source/SolidWireframe/Doc/SolidWireframe.pdf
         */
		ShaderLibraryShaderName_kPointShader,
        /**
         * The shader from this enum will render anti-aliased checkerboard pattern on top of a mesh.
         *
         * The pattern is generated.
         */
		ShaderLibraryShaderName_kUVShader,
        /**
         * The shader from this enum will render a mesh with the selected texture.
         * 
         * The shader wont do any light calculation but just render a mesh with a texture from the material. If there is no texture
         * then it will be simply black(for now). The shader assumes that the albedo map is in sRGB space and it will convert it back to linier
         * if the texture is not albedo/color is understood to be in linier space and it wont be converted to linier.
         */
		ShaderLibraryShaderName_kSingleTextureShader,
        /**
         * The shader from this enum do post-processing on a image/scene.
         * 
         * This shader should be used on a full screen quad because its designed for it. This shader assumes that all of the pixels are
         * in liner color space and at the end will transform them in sRGB. The shader also can do fast approximate anti-aliasing if enabled.
         */
		ShaderLibraryShaderName_kPostProcessShader,
        /**
         * The shader from this enum renders a mesh and colors blue the pixels if the normals are facing the correct way.
         *
         * The shader renders any mesh and determines if the normals are facing the correct way. If they are not then they are colored 
         * red and if they are then they are colored blue.
         */
		ShaderLibraryShaderName_kNormalShader,
	};

    /**
     * @brief A hash map containing all shaders used in the application.
     *
     * This class serves as a named hash map for managing shaders. It stores all of the application's shaders
     * and provides methods for accessing and managing them. Additionally, it is responsible for creating each shader program.
     *
     * The class works with the 'ShaderClass' and the 'ShaderLibraryShaderName_' enum.
     */
	class ShaderLibrary
	{
	public:
        /**
         * @brief Constructor for the ShaderLibrary class.
         *
         * Constructor creates all of the shaders used by the application.   
         */
		ShaderLibrary();
        /**
         * @brief Retrieves a reference to a shader in the library.
         *
         * @param nameOfShader The enum value representing the name of the shader to retrieve.
         * @return ShaderClass& A reference to the ShaderClass object corresponding to the specified shader.
         *
         * @note This method asserts that the shader exists in the library before attempting to return it.
         */
		ShaderClass& GetShaderRef(ShaderLibraryShaderName_ nameOfShader) {assert(isShaderPresent(nameOfShader)); return m_shaderLibraryHash.at(nameOfShader); }
        /**
         * @brief Retrieves a pointer to a shader in the library.
         *
         * @param nameOfShader The enum value representing the name of the shader to retrieve.
         * @return ShaderClass* A pointer to the ShaderClass object corresponding to the specified shader.
         *
         * @note This method asserts that the shader exists in the library before attempting to return it.
         */
        ShaderClass* GetShaderPtr(ShaderLibraryShaderName_ nameOfShader) {assert(isShaderPresent(nameOfShader)); return &m_shaderLibraryHash.at(nameOfShader); }
        /**
         * @brief Provides read-only access to the shader library hash map.
         *
         * @return const std::unordered_map<ShaderLibraryShaderName_, ShaderClass>& A constant reference to the internal shader library hash map.
         */
        const std::unordered_map< ShaderLibraryShaderName_, ShaderClass>& GetShaderHash()const { return m_shaderLibraryHash; }
	private:
        /**
          * @brief Checks if a shader is present in the library.
          *
          * @param nameOfShader The enum value representing the name of the shader to check.
          * @return bool True if the shader exists in the library, otherwise false.
          */
		bool isShaderPresent(ShaderLibraryShaderName_ nameOfShader) { return m_shaderLibraryHash.find(nameOfShader) != m_shaderLibraryHash.end(); }
	private:
		std::unordered_map< ShaderLibraryShaderName_, ShaderClass> m_shaderLibraryHash; ///< Application shader hash map.
	};

}