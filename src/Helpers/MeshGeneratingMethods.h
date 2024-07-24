#pragma once
#include"Scene/Mesh.h"
namespace OBJ_Viewer {
    /* When it comes to cube it doesn't have many options on how to generate it and because this is not a modeling software 
    * we can just use the model matrix to pre-bake the information from it.
    */
#pragma region  Mesh cube data
    const std::vector<unsigned int> ONE_BY_ONE_CUBE_INDICIES = {
       0, 1, 2, 2, 3, 0,   // Front face
       6, 5, 7, 5, 4, 7,   // Back face
       10, 11, 9, 9, 11, 8, // Left face
       14, 13, 12, 14, 12, 15, // Right face
       16, 17, 18, 18, 19, 16, // Top face
       22, 21, 20, 22, 20, 23  // Bottom face
    };
    const std::vector<glm::vec3> ONE_BY_ONE_CUBE_POSITION_VECTOR = {
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
        {  1.0f, -1.0f, -1.0f}}; 
    const std::vector<Vertex> CUBE_VERTEX_VECTOR = {
       //TODO:Calc the tangent vector of the cube.
        // Front face
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[0], glm::vec2(0.0f, 0.0f), { 0.0f,  0.0f,   1.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[1], glm::vec2(1.0f, 0.0f), { 0.0f,  0.0f,   1.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[2], glm::vec2(1.0f, 1.0f), { 0.0f,  0.0f,   1.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[3], glm::vec2(0.0f, 1.0f), { 0.0f,  0.0f,   1.0f},glm::vec3(0)},

        // Back face
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[4], glm::vec2(0.0f, 0.0f), { 0.0f,  0.0f, -1.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[5], glm::vec2(1.0f, 0.0f), { 0.0f,  0.0f, -1.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[6], glm::vec2(1.0f, 1.0f), { 0.0f,  0.0f, -1.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[7], glm::vec2(0.0f, 1.0f), { 0.0f,  0.0f, -1.0f},glm::vec3(0)},

        // Left face
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[8], glm::vec2(0.0f, 0.0f), {-1.0f,  0.0f,   0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[9], glm::vec2(1.0f, 0.0f), {-1.0f,  0.0f,   0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[10], glm::vec2(1.0f, 1.0f), {-1.0f,  0.0f,  0.0f}, glm::vec3(0)},
        { ONE_BY_ONE_CUBE_POSITION_VECTOR[11], glm::vec2(0.0f, 1.0f), {-1.0f,  0.0f,  0.0f}, glm::vec3(0)},

        // Right face
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[12], glm::vec2(0.0f, 0.0f), { 1.0f,  0.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[13], glm::vec2(1.0f, 0.0f), { 1.0f,  0.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[14], glm::vec2(1.0f, 1.0f), { 1.0f,  0.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[15], glm::vec2(0.0f, 1.0f), { 1.0f,  0.0f,  0.0f},glm::vec3(0)},

        // Top face
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[16], glm::vec2(0.0f, 0.0f), { 0.0f,  1.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[17], glm::vec2(1.0f, 0.0f), { 0.0f,  1.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[18], glm::vec2(1.0f, 1.0f), { 0.0f,  1.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[19], glm::vec2(0.0f, 1.0f), { 0.0f,  1.0f,  0.0f},glm::vec3(0)},

        // Bottom face
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[20], glm::vec2(0.0f, 0.0f), { 0.0f, -1.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[21], glm::vec2(1.0f, 0.0f), { 0.0f, -1.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[22], glm::vec2(1.0f, 1.0f), { 0.0f, -1.0f,  0.0f},glm::vec3(0)},
        {ONE_BY_ONE_CUBE_POSITION_VECTOR[23], glm::vec2(0.0f, 1.0f), { 0.0f, -1.0f,  0.0f},glm::vec3(0)},
    };
#pragma endregion
 
#pragma region Mesh plabe data
    const std::vector<glm::vec3> PLANE_POSITION_DATA = 
    {
         { -1.0f,1.0f,0.0f },
         {-1.0f,-1.0f, 0.0f },
         { 1.0f, 1.0f, 0.0f },
         { 1.0f,-1.0f, 0.0f }
    };
    const std::vector<unsigned int> PLANE_INDEX_DATA = { 0, 1, 2, 2, 1, 3 };

#pragma endregion

    inline std::unique_ptr<VertexAttributeObject> GenerateCubeVAO(bool isOnlyPosition = false)
    {   
        return  isOnlyPosition ? std::make_unique<VertexAttributeObject>(ONE_BY_ONE_CUBE_POSITION_VECTOR, ONE_BY_ONE_CUBE_INDICIES) :
            std::make_unique<VertexAttributeObject>(CUBE_VERTEX_VECTOR, ONE_BY_ONE_CUBE_INDICIES);
    }
    inline std::unique_ptr<VertexAttributeObject> GeneratePlaneVAO()
    {
        return std::make_unique<VertexAttributeObject>(PLANE_POSITION_DATA, PLANE_INDEX_DATA);
    }
    /* Because this function is used only in the "SceneRenderer" class I don't really want to heap alloc here.
    * Using RVO we can return this object to the caller without any copies basically "constructing" it in place.
    * One thing that might fail here is if the compiler don't support RVO in that case this wont work and will copy 
    * the temp object. And because is shallow copy our GPU side data will be gone.
    */
    inline VertexAttributeObject GeneratePlaneVAOStack()
    {
        return VertexAttributeObject(PLANE_POSITION_DATA, PLANE_INDEX_DATA);
    }
}