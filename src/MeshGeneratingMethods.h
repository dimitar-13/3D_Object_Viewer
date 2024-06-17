#pragma once
#include"Scene/Mesh.h"
namespace OBJ_Viewer {


    /* When it comes to cube it doesn't have many options on how to generate it and because this is not a modeling software 
    * we can just use the model matrix to do our uniform scale.
    */

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
 
    inline std::unique_ptr<Mesh> GenerateCubeMesh(bool isOnlyPosition = false)
    {   
        return  isOnlyPosition ? std::make_unique<Mesh>(ONE_BY_ONE_CUBE_POSITION_VECTOR, ONE_BY_ONE_CUBE_INDICIES) :
            std::make_unique<Mesh>(CUBE_VERTEX_VECTOR, ONE_BY_ONE_CUBE_INDICIES);
    }
}