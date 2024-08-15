#pragma once
#include <glm/glm.hpp>
#include <array>
#include "Enums/MaterialFlags.h"
namespace OBJ_Viewer {
    namespace APP_SETTINGS {
        /**
         * Current std140 layout rules:
         * 
         * -------------!IMPORTANT-------------
         * These rules are gotten from this website : https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL 
         * and are directly copied I haven't written them:
         * -------------------------------------
         * 
         * - **Scalar Types** (e.g., int, bool):
         *   - **Base Alignment**: N
         *   - Scalars are aligned to the base alignment size N.
         * 
         * - **Vectors**:
         *   - **Base Alignment**: 2N or 4N
         *   - For example, a `vec3` (3-component vector) has a base alignment of `4N`. 
         *     This ensures that vectors are aligned to the larger boundary for efficient access.
         * 
         * - **Arrays of Scalars or Vectors**:
         *   - **Base Alignment**: Same as `vec4`
         *   - Each element in the array follows the alignment rules of `vec4`. 
         *     This means the entire array aligns to `vec4` boundaries.
         * 
         * - **Matrices**:
         *   - **Storage**: Large array of column vectors
         *   - **Base Alignment**: `vec4`
         *   - Matrices are stored as arrays of column vectors, where each column vector aligns to `vec4`.
         * 
         * - **Structs**:
         *   - **Size Calculation**: Computed based on the size and alignment of its elements.
         *   - **Padding**: Padded to a multiple of the size of `vec4`.
         *   - Structs are aligned to ensure that their size is a multiple of `vec4`, which is essential for proper memory access.
         */

        /**
         * @brief The shading model used by the light calculation.
         *
         * The shading model to use when doing the light calculation.
         *  - LightShadingModel_kUnknown: For debugging.
         *  - LightShadingModel_kBlinPhong: Blin-Phong light shading model.
         *  - LightShadingModel_kToonShading: Toon light shading model.
         *  - LightShadingModel_kRimShading: Rim light shading model.
         *  - LightShadingModel_kRim_and_ToonShading: Rim + Toon shading light model.
         * 
         * The user can switch between these modes via the UI.
         */
        enum LightShadingModel_
        {
            LightShadingModel_kUnknown,              ///< For debugging.
            LightShadingModel_kBlinPhong,            ///< Regular light calculation using ambient + diffuse + specular.
            LightShadingModel_kToonShading,          ///< Toon shading model. Only calculates diffuse lighting, using a floor function on the diffuse dot product to create a cartoon-like effect without specular highlights.
            LightShadingModel_kRimShading,           ///< Rim shading model. Applies a rim light effect around the edges of objects to create a glowing outline.
            LightShadingModel_kRim_and_ToonShading   ///< Combines the rim and toon shading.
        };

        /**
         * @brief Enum representing the current shading technique.
         *
         * The struct represent the shading technique that the user has selected.
         *  - RenderingMode_kUnknown: For debugging.
         *  - RenderingMode_kWireframe: Solid wireframe(lines or points).
         *  - RenderingMode_kSolidColor: Studio light solid color.
         *  - RenderingMode_kLight: Shading with light calculations.
         *  - RenderingMode_kUV: Shading with checkerboard pattern to show UV's.
         *  - RenderingMode_kIndividualTexture: Shading by showing only 1 texture with no light calculations.
         *  - RenderingMode_kNormalOrientation: Shading by coloring the pixels based on their normal orientation.
         */
        enum RenderingMode_
        {
            RenderingMode_kUnknown,            ///< For debugging.
            RenderingMode_kWireframe,          ///< Solid wireframe(lines or points).
            RenderingMode_kSolidColor,         ///< Studio light solid color.
            RenderingMode_kLight,              ///< Shading with light calculations.
            RenderingMode_kUV,                 ///< Shading with checkerboard pattern to show UV's.
            RenderingMode_kIndividualTexture,  ///< Shading by showing only 1 texture with no light calculations.
            RenderingMode_kNormalOrientation   ///< Shading by coloring the pixels based on their normal orientation.
        };

        /**
         * @brief Structure for configuring grid settings via the user interface.
         *
         * This structure encapsulates the settings used to render the grid in the application. It allows users to customize
         * various aspects of the grid's appearance and behavior through the UI.
         *
         * The structure includes:
         * - Grid Color: The color of the grid lines.
         * - Grid Scale: The scale of the grid, affecting the size of each grid square.
         * - Axis Shading: A flag indicating whether shading is applied to the X and Z axes.
         * - Minimum and Maximum Grid Scale: Constants defining the permissible range for the grid scale.
         *
         * @note The grid scale is adjustable within a specified range, defined by `kGridMinScale` and `kGridMaxScale`.
         */
        struct GridData
        {
            static constexpr float kGridMinScale = 1.0f; ///< This constant defines the smallest scale value that the grid can be set to. It represents the largest grid squares.
            static constexpr float kGridMaxScale = 5.0f; ///< This constant defines the largest scale value that the grid can be set to. It represents the smallest grid squares.

            float gridScale = kGridMinScale;            ///< The scale of the grid(the more scale the smaller each grid square is.)
            glm::vec3 gridLineColor = glm::vec3(.5);    ///< The grid color.
            bool isAxisShaded = true;                   ///< Flag for enabling or disabling the axis shading(X and Z axis).
        };

        /**
         * @brief Structure representing a single directional light.
         * 
         * The structure contains information about:
         *  - Light direction: The direction of the light.
         *  - Light color: The color of the light.
         * 
         * @note Because the directional lights are submitted with Uniform buffer object(UBO) we have to add the 2 float paddings in between
         * the direction and color variables see the std140 layout data rules for more info ^^^.
         */
        struct DirectionalLight
        {
            glm::vec3 direction = glm::vec3(0); ///< The direction of the light.
            float padding1;                     ///< Related to the std140 layout rules.
            glm::vec3 color = glm::vec3(0);     ///< The color of the light.
            float padding2;                     ///< Related to the std140 layout rules.
        };

        /**
         * @brief Structure for configuring wireframe rendering settings.
         *
         * This structure defines the settings used for rendering wireframes in the application. It allows customization
         * of various visual aspects of the wireframe, such as line thickness, color, and a flag to switch from line to point rendering.
         *
         * The structure includes:
         * - Line Thickness: Specifies the width of the wireframe lines.
         * - Line Color: Defines the color of the wireframe lines.
         * - Point Rendering: A flag indicating whether to render points at the vertices of the wireframe.
         */
        struct WireFrameSettings
        {
            float lineThickness = 1.f;                  ///< Thickness of the line or point.
            glm::vec3 lineColor = glm::vec3(0, 1, 0);   ///< Color of the line or point.
            bool isPointRenderingOn = false;            ///< Flag that indicates if we are rendering points or lines.
        };

        /**
         * @brief Structure representing scene light information.
         *
         * This structure holds information about the lights used in the scene, including the number of lights,
         * their properties, and the shading model applied to them.
         */
        struct SceneLightInfo
        {
            static constexpr size_t kMaxLightCount = 4;                             ///< Max application light count.
            static constexpr size_t kSizeOfLightInBytes = sizeof(DirectionalLight); ///< Size of a single light in bytes.
            int lightCount = 1;                                                     ///< Current count of enabled light.Max count is defined by 'kMaxLightCount'.
            std::array<DirectionalLight, kMaxLightCount>lights = {                  ///< Lights array.
                DirectionalLight{glm::vec3(1),0,glm::vec3(1),0},
                DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
                DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
                DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
            };
            LightShadingModel_ currentLightModel = LightShadingModel_kBlinPhong;    ///< Current light shading model see 'LightShadingModel_' for the available one.
        };

        /**
         * @brief Structure for configuring UV view settings in the application.
         *
         * This structure contain configuration data for generating a UV(checkerboard pattern) onto the currently loaded mesh.
         *
         * The structure includes:
         * -UV Scale Factor: The scaling factor applied to the UV coordinates.
         * -Minimum and Maximum UV Scaling: Constants defining the allowable range for UV scaling.
         */
        struct UV_ViewAppSetting
        {
            static constexpr float kUV_MaxScaling = 100.f;  ///< Max UV scaling factor.
            static constexpr float kUV_MinScaling = 30.0f;  ///< Min UV scaling factor.
            float UV_scaleFactor = kUV_MinScaling;          ///< The UV scaling factor(more scale smaller squares).
        };

        /**
         * @brief Structure combining all application configuration settings into a single structure.
         *
         * This structure holds various settings that configure how the scene is rendered and displayed in the application.
         * It includes flags for enabling or disabling features, rendering modes, material settings, and light configurations.
         */
        struct SceneConfigurationSettings
        {
            bool IsWireGridOn = false;                                              ///< Should we render the infinite X and Z grid.
            bool IsSkyboxOn = false;                                                ///< If enabled should we render them skybox.
            bool EnableScneAntiAliasing = true;                                     ///< Should we do scene Anti-Aliasing. 
            RenderingMode_ CurrentRenderingMode = RenderingMode_kSolidColor;        ///< Currently selected rendering mode see 'RenderingMode_'. 
            glm::vec3 StudioLightShadingColor = glm::vec3(1);                       ///< Currently configured studio light shading color. 
            MaterialRenderingFlags_ MaterialFlags = MaterialRenderingFalgs_kAll;    ///< Currently configured material flags used for light rendering.
            MaterialTextures_ CurentIndividualTexture = MaterialTextures_kAlbedo;   ///< Currently selected texture for the single texture rendering.
            UV_ViewAppSetting UVViewSettings;                                       ///< Uv settings for UV rendering.    
            GridData GridData;                                                      ///< Grid configuration settings.
            SceneLightInfo LightInfo;                                               ///< Scene light information.
            WireFrameSettings WireframeSettings;                                    ///< Solid wireframe settings for wireframe rendering.
        };


        /**
         * @brief A default loading mesh type.
         *
         * Currently not used but reserved.
         */
        enum BaseMeshType_
        {
            BaseMeshType_kCube = 0,
            BaseMeshType_kSphere = 1,
            BaseMeshType_kTorus = 2
        };
    }
}