#pragma once
#include "pch.h"
#include "gpu_side/Texture.h"
#include "Scene/Material.h"
#include "Scene/Model.h"
#include "FileFromatStruct.h"
#include "Scene/MaterialRegistry.h"

namespace OBJ_Viewer
{
	struct BoundingBox
	{
		glm::vec3 max = glm::vec3(-std::numeric_limits<float>::min());
		glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
	};
	struct ReadMeshData
	{
		unsigned int mesh_material_ID;
		ModelVertexData mesh_Information;
		std::vector<Vertex> vertex_data_vector;
		std::vector<VertexAttributeObject::IndexDataType> index_data_vector;
		BoundingBox mesh_bounding_box;
	};
    /**
     * @brief A class that reads a 3D scene/model using Assimp.
     *
     * This class utilizes the Assimp library to load and parse 3D scenes or models, handling the complexities
     * of reading various file formats (e.g., OBJ, FBX). It abstracts the reading logic and creates and sets the dynamic
     * memory allocation for two key components: `Model` and `MaterialRegistry`.
     *
     * The dynamically allocated memory for these components is not managed by this class, meaning the caller
     * is responsible for retrieving and freeing the memory. Use the methods `GetLoadedModel` and
     * `GetLoadedMaterialRegistry` to access the loaded data.
     */
	class ModelLoader
	{
	public:
        /**
         * @brief Constructs a ModelLoader that reads a 3D scene file (obj, fbx, etc.) and creates
         *        a Model and a MaterialRegistry objects.
         *
         * This constructor initializes the ModelLoader with the specified file path and file type,
         * which are used to load and parse the 3D scene file.
         *
         * @param modelFilePath Path to the 3D scene file (obj, fbx, etc.).
         * @param modelFileType File type of the 3D model to be loaded. See LoadModelFileType_
         *                      for the currently supported file extensions.
         */
        ModelLoader(const char* modelFilePath, LoadModelFileType_ modelFileType);
        /**
        * @brief Returns a owning pointer to the loaded 3D scene/Model.
        * 
        * This method returns a std::unique_ptr<Model> to the loaded 3D scene/Model if the loading proccess failed it
        * will return an empty std::unique_ptr<Model>. You can use "isFileLoadedSuccessfully" to check if the file reading failed.
        */
		std::unique_ptr<Model> GetLoadedModel() { return std::unique_ptr<Model>(m_loadedScene); }
        /**
        * @brief Returns a owning pointer to the loaded MaterialRegistry.
        *
        * This method returns a std::unique_ptr<MaterialRegistry> to the loaded MaterialRegistry if the loading proccess failed it
        * will return an empty std::unique_ptr<MaterialRegistry>. You can use "isFileLoadedSuccessfully" to check if the file reading failed.
        */
		std::unique_ptr<MaterialRegistry> GetLoadedMaterialRegistry() { return std::unique_ptr<MaterialRegistry>(m_materialRegistry); }
        /**
        * @brief Returns true if file was loaded without any errors otherwise returns false.
        * 
        * If the file was not valid or assimp failed to find the path this will return false.
        */
		bool isFileLoadedSuccessfully() { return (m_loadedScene != nullptr); }
	private:
        /*
        * Invokes the 'ReadNode' method
        */
		void ReadSceneFile(aiNode* node, const aiScene* scene);
        /**
         * @brief Recursively reads a node-based hierarchy in a 3D scene.
         *
         * This method processes a node-based hierarchy where each node has a parent/child relationship.
         * The reading of each node is performed asynchronously, and the results are stored in the
         * `m_meshThreadResults` member vector.
         *
         * The method calls `ReadMesh` internally, which handles the conversion of Assimp data into
         * the application's vertex and index data format.
         *
         * @param node The current node to be processed.
         * @param scene The Assimp scene representing the imported 3D file.
         */
		void ReadNode(aiNode* node,const aiScene* scene);
		void PostProcessScene();
        /**
         * @brief Reads and converts Assimp mesh data into `ReadMeshData`.
         *
         * This method processes the provided Assimp mesh by applying the given transformation matrix
         * to the vertex data, including both positions and normals. The transformed vertex and index data along side
         * the max and min x, y, z coordinates are stored in a `ReadMeshData` 
         * structure for later use in creating vertex attribute objects.
         *
         * Each invocation of this method is asynchronous, allowing for concurrent processing of multiple meshes.
         *
         * Additionally, this method calculates and stores the minimum and maximum x, y, z coordinates
         * of all vertices in the mesh, which can be used for scene post-processing.
         *
         * @param assimpMesh The Assimp mesh that is currently being processed.
         * @param MeshTransform The transformation matrix (model matrix) to be applied to the mesh.
         * @return A `ReadMeshData` structure containing the processed vertex and index data.
         */
		ReadMeshData ReadMesh(const aiMesh* assimpMesh,const glm::mat4 MeshTransform);
		std::vector<std::shared_ptr<Material>> LoadSceneMaterials(const aiScene* scene);
		std::unique_ptr<std::vector<Mesh>> CreateMeshArray();
		std::shared_ptr<OBJ_Viewer::Texture> ReadTexture(aiMaterial* mat, aiTextureType type);
		std::string GetTrueTexturePathString(const aiString& texturePath)const;
        /**
         * @brief Converts an Assimp 4x4 matrix to a `glm::mat4` object.
         *
         * This function converts the given Assimp 4x4 matrix (`aiMatrix4x4`) into a `glm::mat4` object.
         * If the provided Assimp matrix is an identity matrix, the function returns a `glm::mat4` identity matrix.
         *
         * @param matrix The Assimp matrix (`aiMatrix4x4`) to be converted.
         * @return A `glm::mat4` object representing the converted matrix.
         */
		glm::mat4 AssimpToGlmMatrix4x4(const aiMatrix4x4& matrix);
	private:
		struct TypeMaterialRepresentation
		{
			TypeMaterialRepresentation(LoadModelFileType_ typeOFModel)
			{
				switch (typeOFModel)
				{
			
				case OBJ_Viewer::LoadModelFileType_kOBJ:
					colorTextureEnum = aiTextureType::aiTextureType_DIFFUSE;
					normalTextureEnum = aiTextureType::aiTextureType_HEIGHT;
					ambientOcclusionEnum = aiTextureType::aiTextureType_AMBIENT_OCCLUSION;
					specularRoughnessEnum = aiTextureType::aiTextureType_SPECULAR;
					break;
				case OBJ_Viewer::LoadModelFileType_kFBX:
					colorTextureEnum = aiTextureType::aiTextureType_DIFFUSE;
					normalTextureEnum = aiTextureType::aiTextureType_NORMALS;
					ambientOcclusionEnum = aiTextureType::aiTextureType_AMBIENT_OCCLUSION;
					specularRoughnessEnum = aiTextureType::aiTextureType_METALNESS;
					break;

				case OBJ_Viewer::LoadModelFileType_kUnknown: default:
					colorTextureEnum = aiTextureType::aiTextureType_UNKNOWN;
					normalTextureEnum = aiTextureType::aiTextureType_UNKNOWN;
					ambientOcclusionEnum = aiTextureType::aiTextureType_UNKNOWN;
					specularRoughnessEnum = aiTextureType::aiTextureType_UNKNOWN;
					break;
				}
			}
			aiTextureType colorTextureEnum;
			aiTextureType normalTextureEnum;
			aiTextureType ambientOcclusionEnum;
			aiTextureType specularRoughnessEnum;
		};
	private:
		MaterialRegistry* m_materialRegistry = nullptr;
		Model* m_loadedScene = nullptr;
		ModelUIData m_ModelData;
		std::string m_modelPath;
		LoadModelFileType_ m_currentlyLoadingType;
		std::future<std::vector<std::shared_ptr<Material>>> m_materialRegistryThread;
		std::vector<std::future<ReadMeshData>> m_meshThreadResults;
#pragma region Post-proccess variables
		BoundingBox m_sceneBoundingBox{};
		glm::mat4 m_SceneAppNormalizeMatrix = glm::mat4(1);;
#pragma endregion
	};
}

