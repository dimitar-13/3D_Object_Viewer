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
		MeshInfo mesh_Information;
		std::vector<Vertex> vertex_data_vector;
		std::vector<VertexAttributeObject::IndexDataType> index_data_vector;
		BoundingBox mesh_bounding_box;
	};
	
	class ModelLoader
	{
	public:
		ModelLoader(const char* modelFilePath, LoadModelFileType_ modelFileType);
		std::unique_ptr<Model> GetLoadedModel() { return std::unique_ptr<Model>(m_loadedScene); }
		std::unique_ptr<MaterialRegistry> GetLoadedMaterialRegistry() { return std::unique_ptr<MaterialRegistry>(m_materialRegistry); }
		bool isFileLoadedSuccessfully() { return (m_loadedScene != nullptr); }
	private:
		void ReadSceneFile(aiNode* node, const aiScene* scene);
		void ReadNode(aiNode* node,const aiScene* scene);
		void PostProcessScene();
		ReadMeshData ReadMesh(const aiMesh* assimpMesh,const glm::mat4 MeshTransform);
		std::vector<std::shared_ptr<Material>> LoadSceneMaterials(const aiScene* scene);
		std::unique_ptr<std::vector<Mesh>> CreateMeshArray();
		std::shared_ptr<OBJ_Viewer::Texture> ReadTexture(aiMaterial* mat, aiTextureType type);
		std::string GetTrueTexturePathString(const aiString& texturePath)const;
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

				case OBJ_Viewer::LoadModelFileType_kUknown: default:
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
		ModelData m_ModelData;
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

