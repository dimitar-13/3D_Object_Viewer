#pragma once
#include "pch.h"
#include "gpu_side/Texture.h"
#include "Scene/Material.h"
#include "Scene/Model.h"
#include "FileFromatStruct.h"
#include "Scene/MaterialRegistry.h"

namespace OBJ_Viewer
{
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
    private:
#pragma region Loder specific structs
        /**
         * @brief Structure representing a 3D bounding box.
         *
         * This structure defines a bounding box in 3D space using minimum and maximum corner points.
         * The default values for `min` and `max` are set to encompass the entire range of float values,
         * effectively representing an infinite bounding box until updated with actual values.
         */
        struct BoundingBox
        {
            glm::vec3 max = glm::vec3(-std::numeric_limits<float>::min()); ///< Max recorded value.
            glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());  ///< Min record value.
        };

        /**
         * @brief Structure representing the result of a mesh reading operation in a threaded context.
         *
         * This structure is used to hold all relevant data extracted from a mesh file during a threaded read operation.
         * It contains material IDs, vertex and index data, as well as a bounding box for the mesh.
         */
        struct ReadMeshThreadResult
        {
            unsigned int mesh_material_ID;                                          ///< The mesh material ID used in the scene file and the MaterialRegistry.
            ModelVertexData mesh_Information;                                       ///< Mesh information    
            std::vector<Vertex> vertex_data_vector;                                 ///< Mesh vertex data array.
            std::vector<VertexAttributeObject::IndexDataType> index_data_vector;    ///< Mesh index data array.
            BoundingBox mesh_bounding_box;                                          ///< Per-mesh bounding box.
        };
#pragma endregion
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
        * @returns Returns false if the file was not valid or assimp failed to find the path otherwise returns true.
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
        /**
         * @brief Normalizes and scales the scene to fit a specific application size.
         *
         * This function performs several transformations to adjust the scene's scale and position:
         * - Normalization: Computes the bounding box of the scene using its minimum and maximum extents, and normalizes the scene's size to fit within a predefined application-specific size.
         * - Centering: Moves the scene's world position to the coordinates `{0, 1, 0}` to ensure that the scene is centered appropriately within the application.
         * - Model Matrix Update: Applies these transformations to the model matrix, which is used to construct the final model. The model matrix incorporates normalization and positioning adjustments.
         *
         * The final model matrix reflects these transformations, allowing the scene to be properly scaled and positioned within the application context.
         */
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
		ReadMeshThreadResult ReadMesh(const aiMesh* assimpMesh,const glm::mat4 MeshTransform);
        /**
         * @brief Reads and loads all materials from the provided scene.
         *
         * This function iterates through all the materials in the given `aiScene` and attempts to read and load
         * the associated textures for each material. If a texture is successfully read, it is added to the material;
         * otherwise, the function skips the texture. The function specifically handles textures of types defined in
         * the `MaterialTextures_` enumeration.
         *
         * @param scene Pointer to the `aiScene` object that contains the materials to be loaded.
         *
         * @return A vector of shared pointers to `Material` objects, each representing a material with its
         *         associated textures loaded from the scene. If a material does not have textures or fails to load,
         *         it will not be included in the returned vector.
         */
		std::vector<std::shared_ptr<Material>> LoadSceneMaterials(const aiScene* scene);
        /**
         * @brief Creates a array of meshes and returns it.
         *
         * This function gets the result from each of the 'ReadMesh()' and constructs a Mesh class.
         * The function also compares max and the min recorded x,y,z components from each ReadMesh result and gets the biggest and smallest.
         * 
         * This function is not asynchronies because OpenGL API calls are designed to work on a single thread.
         * 
         * @returns A owning pointer to a created Mesh array.
         */
		std::unique_ptr<std::vector<Mesh>> CreateMeshArray();
        /**
         * @brief Reads a texture from the 3D scene file based on the specified texture type.
         *
         * This function attempts to locate and read a texture of a given type from the material data of a loaded 3D scene file.
         *
         * @param mat Pointer to the Assimp material object that contains texture information.
         * @param type The type of the texture to read (e.g., diffuse, specular) as defined by the `aiTextureType` enumeration.
         *
         * @return A `std::shared_ptr<OBJ_Viewer::Texture>` to the successfully read texture. If the texture type does not exist in the material
         *         or if the texture fails to be read, the function returns a null pointer.
         *
         * @note If the specified texture type is not present or the reading process encounters an error, the function will return `nullptr`.
         *       When a texture is successfully read, it is returned as a `shared_ptr` to manage its lifetime properly.
         */
		std::shared_ptr<OBJ_Viewer::Texture> ReadTexture(aiMaterial* mat, aiTextureType type);
        /**
         * @brief Returns the texture path.
         * 
         * This function takes a texture path and check if its absolute if its not it returns the
         * concatenated string (model path + relative to model path texture path).
         *
         * This is done because if the texture is not relative to a 3D model file file. This can happen if the texture, used by the 3D file, is not in
         * the same directory then as the 3D model file. In this case the texture path will be absolute but if not the texture path will be relative. 
         *  This function accounts for that by returning the correct path.
         */
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
        /**
         * @brief Structure representing material texture types based on model file format.
         *
         * This structure maps different model file formats to their corresponding texture types used in materials.
         * It sets the texture types for color, normal, ambient occlusion, and specular roughness based on the model file type.
         */
		struct TypeMaterialRepresentation
		{
            /**
             * @brief Constructs a `TypeMaterialRepresentation` object based on the model file type.
             *
             * The constructor initializes the texture type enums based on the provided model file format:
             * - OBJ: Uses specific texture types associated with OBJ files.
             * - FBX: Uses different texture types associated with FBX files.
             * - Unknown: Used for debugging.
             *
             * @param typeOFModel The type of model file (e.g., OBJ, FBX).
             */
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
			aiTextureType colorTextureEnum;       ///< Texture type for color (diffuse) maps.
            aiTextureType normalTextureEnum;      ///< Texture type for normal maps.
            aiTextureType ambientOcclusionEnum;   ///< Texture type for ambient occlusion maps.
            aiTextureType specularRoughnessEnum;  ///< Texture type for specular and roughness maps.
      
		};
	private:
		MaterialRegistry* m_materialRegistry = nullptr;                                 ///< Pointer to the material registry that is to be loaded.
		Model* m_loadedScene = nullptr;                                                 ///< Pointer to the scene/3D model to be loaded.          
		ModelUIData m_ModelData;                                                        ///< Extracted model UI data.
		std::string m_modelPath;                                                        ///< Received model path with the file name removed(this should be absolute depending on the Constructor call.).
		LoadModelFileType_ m_currentlyLoadingType;                                      ///< Currently loading file type(fbx,obj etc.). See 'LoadModelFileType_' for more information.
		std::vector<std::future<ReadMeshThreadResult>> m_meshThreadResults;             ///< Read mesh threads result.
#pragma region Post-proccess variables
		BoundingBox m_sceneBoundingBox{};                                               ///< Bounding box used for scene post-processing.
		glm::mat4 m_SceneAppNormalizeMatrix = glm::mat4(1);                             ///< Scene normalization matrix created by the 'PostProcessScene()' function.
#pragma endregion
	};
}

