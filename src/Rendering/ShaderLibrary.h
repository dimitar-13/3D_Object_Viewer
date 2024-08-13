#pragma once
#include "pch.h"
#include "gpu_side/ShaderClass.h"
namespace OBJ_Viewer {

    /**
     * @brief Enum used to get shader from the 'ShaderLibrary' class.
     * 
     * - ShaderLibraryShaderName_kUnknown:          For debugging.
     * - ShaderLibraryShaderName_kStudioLightShader: Shader representing studio light effect.  
     * - ShaderLibraryShaderName_kGridShader:        Shader for rendering the grid    
     * - ShaderLibraryShaderName_kSkyboxShader: 
     * - ShaderLibraryShaderName_kLightShader: 
     * - ShaderLibraryShaderName_kWireframeShader: 
     * - ShaderLibraryShaderName_kPointShader: 
     * - ShaderLibraryShaderName_kUVShader: 
     * - ShaderLibraryShaderName_kSingleTextureShader: 
     * - ShaderLibraryShaderName_kPostProcessShader: 
     * - ShaderLibraryShaderName_kNormalShader: 
     */
	enum ShaderLibraryShaderName_
	{
		ShaderLibraryShaderName_kUnknown,
		ShaderLibraryShaderName_kStudioLightShader,
		ShaderLibraryShaderName_kGridShader,
		ShaderLibraryShaderName_kSkyboxShader,
		ShaderLibraryShaderName_kLightShader,
		ShaderLibraryShaderName_kWireframeShader,
		ShaderLibraryShaderName_kPointShader,
		ShaderLibraryShaderName_kUVShader,
		ShaderLibraryShaderName_kSingleTextureShader,
		ShaderLibraryShaderName_kPostProcessShader,
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