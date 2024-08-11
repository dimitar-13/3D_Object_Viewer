#pragma once
#include "Mesh.h"
#include "pch.h"

namespace OBJ_Viewer {
    /**
     * @brief Structure representing a model vertex count data.
     * 
     * Extracted common 'ModelUIData' data.
     */
	struct ModelVertexData
	{
		size_t vertexCount;///< Vertex count of a model.
		size_t faceCount;///< Face count of a model.
		size_t triangleCount;///< triangle count of a model.
	};

    /**
     * @brief Structure for holding information about a loaded model or scene file.
     *
     * The `ModelUIData` structure provides essential information about a model or scene that has been loaded,
     * which is typically used for display or UI purposes within an application. It encapsulates data related
     * to the model's vertices, textures, and the path from which the model was loaded.
     *
     * This structure is commonly used to provide summary information about the model to users or to integrate
     * model data into a user interface for further interaction or visualization.
     *
     * Members:
     * - `modelVertexCountData`: Contains information about the vertex count of the model.
     * - `textureCount`: The number of textures associated with the model.
     * - `modelPath`: The file path to the model's source file.
     *
     */
	struct ModelUIData
	{
		ModelVertexData modelVertexCountData; ///< Model vertex count structure.
		size_t textureCount; ///< Model/scene loaded texture count.
		std::string modelPath; ///< Loaded model/scene absolute file path.
	};
    /**
     * @brief A class representing a 3D model or scene.
     *
     * The `Model` class encapsulates a collection of meshes and manages their rendering and transformation within a 3D scene.
     * Each `Model` instance holds a set of meshes, a transformation matrix, and additional metadata that defines the model's attributes.
     *
     * This class provides several utility functions to access and manipulate the model's data, including retrieving the mesh data,
     * accessing the transformation matrix, and computing the normal matrix. Additionally, it allows for the decomposition of the transformation
     * matrix into position, rotation, and scale components, and provides functions to apply transformations.
     *
     * The `Model` class is typically used in conjunction with a rendering engine to display 3D objects in a scene.
     * It is designed to be flexible and can be extended or modified to suit specific rendering requirements.
     *
     * Key Features:
     * - Ownership and management of a collection of meshes.
     * - Storage and manipulation of a transformation matrix that defines the model's position, rotation, and scale.
     * - Utility functions for matrix decomposition and transformation.
     *
     * See 'Helpers/ModelLoader' or 'Helpers/ModelGenerator' for example use.
     */
	class Model
	{
	public:
        /**
         * @brief Initializes a model or scene.
         *
         * This constructor initializes a model or scene by taking ownership of a vector of meshes and copying the provided transformation matrix and additional model data.
         * The constructor transfers ownership of the meshes resource, meaning that this class will manage the lifetime of the meshes.
         * The model transformation matrix and the additional model data are copied into the class.
         *
         * @param meshes A `std::unique_ptr<std::vector<Mesh>>` representing a unique pointer to a vector of meshes. Ownership of this resource is transferred to the `Model` class.
         * @param modelMatrix A `const glm::mat4&` representing the transformation matrix for the model. This matrix is copied into the class.
         * @param data A `const ModelData&` representing additional data associated with the model, which is also copied into the class.
         */
		Model(std::unique_ptr<std::vector<Mesh>>& meshes,const glm::mat4& modelMatrix, const ModelUIData& data);
        /**
         * @brief Retrieves the model/scene UI data.
         *
         * @returns Readonly ref access to the model/scene UI data.
         */
		const ModelUIData& GetModelUIData()const { return this->m_CurrentModelUIData; }
        /**
         * @brief Retrieves the vector of meshes associated with the model or scene.
         *
         * This method provides read-only access to the vector of `Mesh` objects contained within the model or scene.
         * It is primarily used for rendering purposes, allowing the caller to iterate over the meshes for drawing.
         *
         * @returns A const reference to the vector of `Mesh` objects.
         */
		const std::vector<Mesh>& GetModelMeshes()const { return *this->m_meshes; }
        /**
         * @brief Returns the transformation matrix of the model or scene.
         *
         * This method provides access to the transformation matrix, which represents the model's or scene's transformations
         * such as translation, rotation, and scaling. In the current application, this matrix serves as the normalization matrix,
         * ensuring the model fits within the desired coordinate space.
         *
         * For more details on how this matrix is generated, see the `PostProcess` method in the `ModelLoader` class.
         *
         * @returns A const reference to the model or scene's transformation matrix.
         */
		const glm::mat4& GetModelMatrix()const { return m_ModelMatrix; }
        /**
         * @brief Returns the normal matrix derived from the model or scene's transformation matrix.
         *
         * The normal matrix is used to correctly transform normals in the model's space, ensuring that they are not distorted
         * by non-uniform scaling or other transformations applied to the model. It is computed using the expression
         * `transpose(inverse(<model transform>))`, which ensures the proper transformation of surface normals.
         *
         * @returns A 4x4 matrix representing the normal matrix, calculated as the transpose of the inverse of the model's
         * transformation matrix.
         */
		glm::mat4 GetNormalMatrix()const { return glm::transpose(glm::inverse(m_ModelMatrix)); }
        /**
         * @brief Decomposes the model or scene's transformation matrix into its position, rotation, and scale components.
         *
         * This function extracts the translation (position), rotation, and scale components from the model's transformation matrix.
         * The decomposition allows you to individually access the position, rotation, and scale factors applied to the model.
         * This is mainly used in the UI. 
         *
         * @param[out] pPosition A reference to a `glm::vec3` variable where the extracted position (translation) vector will be stored.
         * @param[out] pRotation A reference to a `glm::vec3` variable where the extracted rotation vector (in Euler angles) will be stored.
         * @param[out] pScale A reference to a `glm::vec3` variable where the extracted scale vector will be stored.
         *
         * @note Currently is not used and not implemented correctly regarding the angles.
         */
		void GetMatrixDecomposed(glm::vec3& pPosition, glm::vec3& pRotation, glm::vec3& pScale)const;
        /**
         * @brief Applies a transformation to the model or scene, including translation, scaling, and rotation.
         *
         * This function applies a transformation to the model or scene by modifying its transformation matrix.
         * The transformation includes translation, scaling, and rotation around a specified axis.
         * Although this function is not currently used in the application, it is intended to be utilized primarily by the UI for transformation operations.
         *
         * @param transform A `glm::vec3` representing the translation vector to be applied to the model.
         * @param scale A `glm::vec3` representing the scale factors along the x, y, and z axes.
         * @param axieToRotate A `glm::vec3` representing the axis around which the rotation will occur.
         * @param rotationAngle A `float` representing the angle of rotation in degrees.
         *
         * @note This function modifies the model's transformation matrix, combining translation, scaling, and rotation into a single transformation.
         *       It is designed with UI-based transformations in mind, such as those triggered by user interactions.
         */
		void ApplyTransformation(glm::vec3 transform, glm::vec3 scale, glm::vec3 axieToRotate, float rotationAngle);
	private:
		ModelUIData m_CurrentModelUIData; /// < Model/scene UI data.
		glm::mat4 m_ModelMatrix; ///< Model/scene transform matrix.
		std::unique_ptr<std::vector<Mesh>> m_meshes; ///< Owning pointer to the mesh vector. 
	};
}

