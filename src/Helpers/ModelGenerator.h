#pragma once
#include "Scene/Model.h"
#include <memory>
namespace OBJ_Viewer {

    /**
     * @brief Static class for generating 'Model' and 'VertexAttributeObject' classes.
     *
     * Currently the class can generate normalize device coordinate cube and plane VertexAttributeObject data.
     * You can use the static methods to generate the desired mesh type.
     * GenerateXXX returns a owning pointer to either 'Model' or 'VertexAttributeObject'.
     * 
     * @note One of the GenerateXXX methods rely on return value optimization witch is the 'GeneratePlaneVAOStack()'
     *       if your compiler don't support it use the unique_ptr variant.
     */
    class ModelGenerator
    {
    public:
        /**
         * @brief Generates a predefined 'Model' representing a cube and returns a unique pointer to it.
         *
         * This static method creates a `Model` instance with predefined data that represents a cube. The cube's
         * vertex data, texture coordinates, and other attributes are hardcoded into the method. The generated model
         * is then wrapped in a `std::unique_ptr` and returned to the caller.
         *
         * The method is useful for quickly generating a basic cube model for testing or debugging purposes, without
         * needing to load or define the cube's geometry manually. The method is currently used as the default application model.
         *
         * @returns A owning pointer to a `Model` object that represents a cube.
         *
         * Example Usage:
         * ```cpp
         * std::unique_ptr<Model> cubeModel = Model::GenerateCubeModel();
         * ```
         */
        static std::unique_ptr<Model> GenerateCubeModel();
        /**
         * @brief Generates a predefined 'VertexAttributeObject' for a cube and returns a unique pointer to it.
         *
         * This static method creates a `VertexAttributeObject` instance with predefined vertex attribute data representing
         * a cube. The cube's vertex data, including positions and optionally other attributes like texture coordinates,
         * are hardcoded into the method. The generated VAO is then wrapped in a `std::unique_ptr` and returned to the caller.
         *
         * The `isOnlyPosition` parameter determines whether only the vertex positions should be included in the VAO. If
         * `isOnlyPosition` is set to `true`, the VAO will only include vertex position attributes, omitting other attributes
         * like texture coordinates or normals.
         *
         * @param isOnlyPosition A boolean flag indicating whether to include only vertex position attributes. Default is `false`.
         *
         * @returns A owning pointer to a `VertexAttributeObject` that represents a cube with the specified attributes.
         */
        static std::unique_ptr<VertexAttributeObject> GenerateCubeVAO(bool isOnlyPosition = false);
        /**
         * @brief Generates a predefined 'VertexAttributeObject' for a plane and returns a unique pointer to it.
         *
         * This static method creates a `VertexAttributeObject` instance with predefined vertex attribute data representing
         * a plane. The plane's vertex data, including positions, texture coordinates, and other attributes, are hardcoded
         * into the method. The generated VAO is then wrapped in a `std::unique_ptr` and returned to the caller.
         *
         * @returns A owning pointer to a `VertexAttributeObject` that represents a plane.
         *
         */
        static std::unique_ptr<VertexAttributeObject> GeneratePlaneVAO();
        /**
         * @brief An optimized version of 'GeneratePlaneVAO()' that don't allocate on the heap.
         * 
         * @note This version rely on return value optimization(RVO) if you compiler don't support this optimization
         *       this function wont work and you will get a error that you cant copy 'VertexAttributeObject'.
         *       In this case use the unique_ptr variant('GeneratePlaneVAO()').
         */
        static VertexAttributeObject GeneratePlaneVAOStack();
    };
}

