#pragma once
namespace OBJ_Viewer {

    /**
     * @brief Enum representing the types of material textures supported by the application.
     *
     * This enum defines various texture types that can be associated with materials in the application. Each enum value
     * corresponds to a specific type of texture used in material rendering.
     * You can think of this enum as a named texture keys.
     *
     * - `MaterialTextures_kUnknown`: Represents an unknown or unspecified texture type. This is often used as a default or fallback.
     * - `MaterialTextures_kAlbedo`: Represents the albedo or diffuse texture, which defines the base color of the material.
     * - `MaterialTextures_kNormal`: Represents the normal map texture, which provides surface detail by altering the surface normals.
     * - `MaterialTextures_kMetallic`: Represents the metallic map texture, which defines the metallic properties of the material.
     * - `MaterialTextures_kAmbientOcclusion`: Represents the ambient occlusion texture, which simulates how ambient light is occluded by nearby geometry.
     */
    enum MaterialTextures_
    {
        MaterialTextures_kUnknown,          ///< Used for debugging.
        MaterialTextures_kAlbedo,           ///< Represents albedo(color) texture in a material.
        MaterialTextures_kNormal,           ///< Represents normal texture in a material.
        MaterialTextures_kMetalic,          ///< Represents metallic texture in a material. 
        MaterialTextures_kAmbientOcclusion  ///< Represents ambient occlusion texture in a material. 
    };

    /**
     * @brief Rendering flags used to enable or disable certain textures upon light rendering. 
     * 
     * This is used mainly by the UI and the rendering stages of the application.
     * The user can submit thse flags ,using the UI, and the renderer uses this information to enable or disable certain textures.
     *
     * - `MaterialRenderingFlags_kNone`: No texture flags are enabled.
     * - `MaterialRenderingFlags_kAlbedoIsOn`: Enables the albedo (diffuse) texture for rendering.
     * - `MaterialRenderingFlags_kNormalMapIsOn`: Enables the normal map texture for rendering, adding surface detail.
     * - `MaterialRenderingFlags_kAmbientOcclusionIsOn`: Enables the ambient occlusion texture to simulate light occlusion.
     * - `MaterialRenderingFlags_kSpecularIsOn`: Enables the specular texture for rendering, affecting the shininess of the material.
     * - `MaterialRenderingFlags_kAll`: Enables all textures (albedo, normal map, ambient occlusion, and specular) for rendering.
     *
     */
    enum MaterialRenderingFlags_
    {
        MaterialRenderingFalgs_kNone = 0,                 ///< No textures are enabled.
        MaterialRenderingFalgs_kAlbedoIsOn = 1,           ///< Enables the albedo(color) texture in the rendering proccess.
        MaterialRenderingFalgs_kNormalMapIsOn = 2,        ///< Enables the normal texture in the rendering proccess.
        MaterialRenderingFalgs_kAmbientOcclusionIsOn = 4, ///< Enables the ambient occlusion texture in the rendering proccess.
        MaterialRenderingFalgs_kSpecularIsOn = 8,         ///< Enables the specular or roughness texture in the rendering proccess.  
        MaterialRenderingFalgs_kAll =                     ///< Enables all of the present material textures in the rendering proccess.       
                                    MaterialRenderingFalgs_kAlbedoIsOn           |
                                    MaterialRenderingFalgs_kNormalMapIsOn        | 
                                    MaterialRenderingFalgs_kAmbientOcclusionIsOn | 
                                    MaterialRenderingFalgs_kSpecularIsOn
    };

}