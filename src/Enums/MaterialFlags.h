#pragma once
namespace OBJ_Viewer {

    enum MaterialTextures_
    {
        MaterialTextures_kUnknown,
        MaterialTextures_kAlbedo,
        MaterialTextures_kNormal,
        MaterialTextures_kMetalic,
        MaterialTextures_kAmbientOcclusion
    };
    enum MaterialRenderingFalgs_
    {
        MaterialRenderingFalgs_kNone = 0,
        MaterialRenderingFalgs_kAlbedoIsOn = 1,
        MaterialRenderingFalgs_kNormalMapIsOn = 2,
        MaterialRenderingFalgs_kAmbientOcclusionIsOn = 4,
        MaterialRenderingFalgs_kSpecularIsOn = 8,
        MaterialRenderingFalgs_kAll = MaterialRenderingFalgs_kAlbedoIsOn | MaterialRenderingFalgs_kNormalMapIsOn | MaterialRenderingFalgs_kAmbientOcclusionIsOn | MaterialRenderingFalgs_kSpecularIsOn
    };

}