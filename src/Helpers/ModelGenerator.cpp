#include "pch.h"
#include "ModelGenerator.h"
#include "Logging/App_Logger.h"
std::unique_ptr<OBJ_Viewer::Model> OBJ_Viewer::ModelGenerator::GenerateCubeModel()
{
    constexpr size_t kCubeVertexCount = 24;
    constexpr size_t kCubeFaceCount = 6;
    constexpr size_t kCubeTriangleCount = 12;
    const char* kDefaultPath = "Unavailable";
    constexpr size_t kDefaultTextureCount = 0;

    constexpr MeshInfo kCubeMeshInfo = { kCubeVertexCount, kCubeFaceCount, kCubeTriangleCount };
    ModelData kModel_data = ModelData{ kCubeMeshInfo,kDefaultTextureCount,kDefaultPath };

    std::unique_ptr<Model> result_model;

    auto model_meshes = std::make_unique<std::vector<Mesh>>();
    model_meshes->emplace_back(GenerateCubeVAO());
    glm::mat4 translation_move_cube_above_ground_plane = glm::translate(glm::mat4(1), glm::vec3(0, 1.f, 0.));
    result_model = std::make_unique<Model>(model_meshes, translation_move_cube_above_ground_plane, kModel_data);

    return result_model;
}

std::unique_ptr<OBJ_Viewer::VertexAttributeObject> OBJ_Viewer::ModelGenerator::GenerateCubeVAO(bool isOnlyPosition)
{
    const std::vector<unsigned int> kCubeIndexVector = {
        0, 1, 2, 2, 3, 0,       // Front face
        6, 5, 7, 5, 4, 7,       // Back face
        10, 11, 9, 9, 11, 8,    // Left face
        14, 13, 12, 14, 12, 15, // Right face
        16, 17, 18, 18, 19, 16, // Top face
        22, 21, 20, 22, 20, 23  // Bottom face
    };
    const std::vector<glm::vec3> kCubePositionVector = {
        // Front face
        { -1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f, 1.0f},
        { 1.0f, 1.0f, 1.0f},
        { -1.0f, 1.0f, 1.0f},
        // Back face
        { -1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 1.0f, 1.0f, -1.0f},
        { -1.0f, 1.0f, -1.0f},
        // Left face
        { -1.0f, -1.0f, -1.0f},
        { -1.0f, -1.0f, 1.0f},
        { -1.0f, 1.0f, 1.0f},
        { -1.0f, 1.0f, -1.0f},
        // Right face
        { 1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, 1.0f},
        { 1.0f, 1.0f, 1.0f},
        { 1.0f, 1.0f, -1.0f},
        // Top face
        { -1.0f,  1.0f, -1.0f},
        { -1.0f, 1.0f, 1.0f},
        {  1.0f, 1.0f, 1.0f},
        {  1.0f, 1.0f, -1.0f},
        // Bottom face
        { -1.0f, -1.0f, -1.0f},
        { -1.0f, -1.0f, 1.0f},
        {  1.0f, -1.0f, 1.0f},
        {  1.0f, -1.0f, -1.0f} };
    const std::vector<Vertex> kCubeVertexVector = {
        // Front face
        {{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},

        // Back face
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},

        // Left face
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},

        // Right face
        {{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},

        // Top face
        {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

        // Bottom face
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    };

    if(!isOnlyPosition)
        return std::make_unique<VertexAttributeObject>(kCubeVertexVector, kCubeIndexVector);
    else
        return std::make_unique<VertexAttributeObject>(kCubePositionVector, kCubeIndexVector);
}

std::unique_ptr<OBJ_Viewer::VertexAttributeObject> OBJ_Viewer::ModelGenerator::GeneratePlaneVAO()
{
    const std::vector<glm::vec3> kPlanePositionData =
    {
         { -1.0f,1.0f,0.0f },
         {-1.0f,-1.0f, 0.0f },
         { 1.0f, 1.0f, 0.0f },
         { 1.0f,-1.0f, 0.0f }
    };
    const std::vector<unsigned int> kPlaneIndexData = { 0, 1, 2, 2, 1, 3 };

    return std::make_unique<VertexAttributeObject>(kPlanePositionData, kPlaneIndexData);
}

OBJ_Viewer::VertexAttributeObject OBJ_Viewer::ModelGenerator::GeneratePlaneVAOStack()
{
    const std::vector<glm::vec3> kPlanePositionData =
    {
         { -1.0f,1.0f,0.0f },
         {-1.0f,-1.0f, 0.0f },
         { 1.0f, 1.0f, 0.0f },
         { 1.0f,-1.0f, 0.0f }
    };
    const std::vector<unsigned int> kPlaneIndexData = { 0, 1, 2, 2, 1, 3 };

    return VertexAttributeObject(kPlanePositionData, kPlaneIndexData);
}
