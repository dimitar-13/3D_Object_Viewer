#pragma once
#include "Scene/Model.h"
#include <memory>
namespace OBJ_Viewer {

    class ModelGenerator
    {
    public:
        static std::unique_ptr<Model> GenerateCubeModel();
        static std::unique_ptr<VertexAttributeObject> GenerateCubeVAO(bool isOnlyPosition = false);
        static std::unique_ptr<VertexAttributeObject> GeneratePlaneVAO();
        static VertexAttributeObject GeneratePlaneVAOStack();
    };
}

