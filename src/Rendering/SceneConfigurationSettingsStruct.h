#pragma once
#include <glm/glm.hpp>
#include <array>
#include "Enums/MaterialFlags.h"
namespace OBJ_Viewer {
    namespace APP_SETTINGS {

        enum LightShadingModel_
        {
            LightShadingModel_kBlinPhong = 0,
            LightShadingModel_kToonShading = 1,
            LightShadingModel_kRimShading = 2,
            LightShadingModel_kRim_and_ToonShading = 3,
            LightShadingModel_kUknown
        };

        enum RenderingMode_
        {
            RenderingMode_kUknown,
            RenderingMode_kWireframe,
            RenderingMode_kSolidColor,
            RenderingMode_kLight,
            RenderingMode_kUV,
            RenderingMode_kIndividualTexture,
            RenderingMode_kNormalOrientation
        };

        struct GridData
        {
            static constexpr float kGridMinScale = 1.0f;
            static constexpr float kGridMaxScale = 5.0f;

            float gridScale = kGridMinScale;
            glm::vec3 gridLineColor = glm::vec3(.5);
            bool isAxisShaded = true;
        };

        struct DirectionalLight
        {
            glm::vec3 direction = glm::vec3(0);
            float padding1;
            glm::vec3 color = glm::vec3(0);
            float padding2;
        };

        struct WireFrameSettings
        {
            float lineThickness = 1.f;
            glm::vec3 lineColor = glm::vec3(0, 1, 0);
            bool isPointRenderingOn = false;
        };
        struct SceneLightInfo
        {
            static constexpr size_t kMaxLightCount = 4;
            static constexpr size_t kSizeOfLightInBytes = sizeof(DirectionalLight);

            int lightCount = 1;
            std::array<DirectionalLight, kMaxLightCount>lights = {
                DirectionalLight{glm::vec3(1),0,glm::vec3(1),0},
                DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
                DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
                DirectionalLight{glm::vec3(0),0,glm::vec3(0),0},
            };
            LightShadingModel_ currentLightModel = LightShadingModel_kBlinPhong;
        };

        struct UV_ViewAppSetting
        {
            static constexpr float kUV_MaxScaling = 100.f;
            static constexpr float kUV_MinScaling = 30.0f;
            float UV_scaleFactor = kUV_MinScaling;
        };
        struct SceneConfigurationSettings
        {
            bool m_isWireGridOn = false;
            bool m_isSkyboxOn = false;
            bool m_EnableAA = true;
            RenderingMode_ m_currentRenderingMode = RenderingMode_kSolidColor;
            glm::vec3 m_colorRenderingColor = glm::vec3(1);
            MaterialRenderingFlags_ m_MaterialFlags = MaterialRenderingFalgs_kAll;

            MaterialTextures_ m_curentIndividualTexture = MaterialTextures_kAlbedo;
            UV_ViewAppSetting m_uvViewSettings;
            GridData m_gridData;
            SceneLightInfo lightInfo;
            WireFrameSettings wireframeSettings;
        };
        enum BaseMeshType_
        {
            BaseMeshType_kCube = 0,
            BaseMeshType_kSphere = 1,
            BaseMeshType_kTorus = 2
        };
    }
}