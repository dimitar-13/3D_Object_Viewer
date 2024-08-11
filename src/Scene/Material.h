#pragma once
#include "gpu_side/Texture.h"
#include "gpu_side/ShaderClass.h"
#include "pch.h"
#include "Enums/MaterialFlags.h"
namespace OBJ_Viewer {

    /**
     * @brief A data structure used for defining a material in the `Material` class.
     *
     * This structure encapsulates the properties of a material, including its color, roughness, and associated textures.
     * The textures are stored in a hash map and accessed using keys from the `MaterialTextures_` enum. Currently, the application
     * does not support multiple textures of the same type (e.g., multiple albedo textures). Not all texture types are required
     * for creating a valid material.
     *
     * - `color`: The base color of the material.
     * - `roughness`: The roughness value of the material, affecting how light interacts with its surface.
     * - `materialTextures`: A hash map where keys are of type `MaterialTextures_` and values are pointers to `Texture` objects.
     *
     * @note The `MaterialTextures_` enum provides keys for different types of textures, such as albedo, normal, and roughness.
     */
	struct MaterialData
	{
		std::unordered_map< MaterialTextures_, std::shared_ptr<Texture>> materialTextures;
		glm::vec3 color = { 1,1,1 };
		float roughness = 64.f;
	};

    /**
     * @brief Class representing a scene-based material.
     *
     * This class wraps `MaterialData` and provides access to material properties such as textures, color, and roughness.
     * A material is characterized by its name and the data defining its properties.
     *
     * Materials are mostly now used on their own see the 'Scene/MaterialRegistry.h' for more information on how the app uses the 
     * materials.
     * @see MaterialData for more detailed information on the material's data structure and properties.
     */
	class Material
	{
	public:
        /**
          * @brief Constructs a Material object with a specified name and optional data.
          *
          * Initializes the material with the given name and material data. If no data is provided, default-initialized
          * `MaterialData` is used.
          *
          * @param materialName The name of the material.
          * @param data Optional material data to initialize the material's properties. Defaults to an empty `MaterialData` object.
          */
		Material(const std::string& materialName, MaterialData data = {}):
			m_materialInfo(data),m_materialName(materialName)
		{}
        /**
         * @brief Gets the count of textures associated with the material.
         *
         * @returns The number of textures defined in the material's data.
         */
		size_t GetMaterialTextureCount()const { return m_materialInfo.materialTextures.size(); }
        /**
         * @brief Retrieves the base color of the material.
         *
         * @returns The base color of the material as a `glm::vec3`.
         */
		glm::vec3 GetMaterialBaseColor()const { return m_materialInfo.color; }
        /**
         * @brief Retrieves the roughness value of the material.
         *
         * @returns The roughness of the material as a `float`.
         */
		float GetMaterialRoughness()const { return m_materialInfo.roughness; }
        /**
         * @brief Retrieves a texture associated with the material.
         *
         * @param texture The type of texture to retrieve, specified by `MaterialTextures_`.
         * @returns A `std::weak_ptr<Texture>` to the requested texture, or an empty `std::weak_ptr` if the texture is not found.
         */
		std::weak_ptr<Texture> GetMaterialTexture(MaterialTextures_ texture)const;
        /**
         * @brief Gets the name of the material.
         *
         * @returns A constant reference to the material's name.
         */
		const std::string& GetMaterialName()const { return m_materialName; }
	private:
		MaterialData m_materialInfo; 
		std::string m_materialName;
	};
}

