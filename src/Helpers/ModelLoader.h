#pragma once
#include "pch.h"
#include "gpu_side/Texture.h"
#include "Scene/Material.h"
#include "Scene/Model.h"
#include "FileFromatStruct.h"
#include "Scene/MaterialRegistry.h"
namespace OBJ_Viewer
{
	class ModelLoader
	{
	public:
		ModelLoader(const char* path, LoadModelFileType modelFileType);
		std::unique_ptr<Model> GetLoadedModel() { return std::unique_ptr<Model>(m_loadedScene); }
		std::unique_ptr<MaterialRegistry> GetLoadedMaterialRegistry() { return std::unique_ptr<MaterialRegistry>(m_materialRegistry); }
		bool isFiledLoadedSuccessfully() { return (m_loadedScene != nullptr); }
	private:
		void ReadNode(aiNode* node,const aiScene* scene);
		std::vector<std::shared_ptr<Mesh>> CreateMeshArray();
		std::shared_ptr<Mesh> ReadMesh(aiMesh* assimpMesh,glm::mat4& MeshTransform);
		void PostProcessScene();
		void LoadSceneMaterials(const aiScene* scene);
		std::shared_ptr<OBJ_Viewer::Texture> ReadTexture(aiMaterial* mat, aiTextureType type);
		std::string GetModelTexturePathAbsolute(aiString texturePath)const;
		glm::mat4 AssimpToGlmMatrix4x4(const aiMatrix4x4& matrix);
	private:
		struct TypeMaterialRepresentation
		{
			TypeMaterialRepresentation(LoadModelFileType typeOFModel)
			{
				switch (typeOFModel)
				{
			
				case OBJ_Viewer::MODEL_TYPE_OBJ:
					colorTextureEnum = aiTextureType::aiTextureType_DIFFUSE;
					normalTextureEnum = aiTextureType::aiTextureType_HEIGHT;
					ambientOcclusionEnum = aiTextureType::aiTextureType_AMBIENT_OCCLUSION;
					specularRoughnessEnum = aiTextureType::aiTextureType_SPECULAR;
					break;
				case OBJ_Viewer::MODEL_TYPE_FBX:
					colorTextureEnum = aiTextureType::aiTextureType_DIFFUSE;
					normalTextureEnum = aiTextureType::aiTextureType_NORMALS;
					ambientOcclusionEnum = aiTextureType::aiTextureType_AMBIENT_OCCLUSION;
					specularRoughnessEnum = aiTextureType::aiTextureType_METALNESS;
					break;

				case OBJ_Viewer::MODEL_TYPE_UKNOWN: default:
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
		std::vector<aiMesh*> m_meshes;
		std::vector<glm::mat4> m_meshTransforms;
		ModelData m_ModelData;
		std::string m_modelPath;
		LoadModelFileType m_currentlyLoadingType;
#pragma region Post-proccess variables
		glm::vec3 m_biggestComponents;
		glm::vec3 m_smallestComponents;
		glm::mat4 m_sceneScaleMatrix;
#pragma endregion
	};
}

