#pragma once
#include<iostream>
#include<glm/glm.hpp>
#include<vector>
#include"Texture.h"
#include"Scene/Material.h"
#include"Scene/Model.h"
#include"FileFromatStruct.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>     // Post processing flags
namespace OBJ_Viewer
{

	struct TypeMaterialRepresentation
	{
		TypeMaterialRepresentation(LoadModelFileType typeOFModel)
		{
			switch (typeOFModel)
			{
			case OBJ_Viewer::MODEL_TYPE_UKNOWN:
				break;
			case OBJ_Viewer::MODEL_TYPE_OBJ:
				colorTextureEnum = aiTextureType_DIFFUSE;
				normalTextureEnum = aiTextureType_HEIGHT;
				ambientOcclusionEnum = aiTextureType_AMBIENT_OCCLUSION;
				specularRoughnessEnum = aiTextureType_SPECULAR;
				break;
			case OBJ_Viewer::MODEL_TYPE_FBX:
				colorTextureEnum = aiTextureType_DIFFUSE;
				normalTextureEnum = aiTextureType_NORMALS;
				ambientOcclusionEnum = aiTextureType_AMBIENT_OCCLUSION;
				specularRoughnessEnum = aiTextureType_METALNESS;
				break;
			default:
				break;
			}
		}
		aiTextureType colorTextureEnum;
		aiTextureType normalTextureEnum;
		aiTextureType ambientOcclusionEnum;
		aiTextureType specularRoughnessEnum;
	};


	class ModelLoader
	{
	public:
		Model* LoadModel(const char* path, LoadModelFileType modelFileType);
	private:
		void ReadNode(aiNode* node,const aiScene* scene);
		std::vector<std::shared_ptr<Mesh>> CreateMeshArray();
		std::shared_ptr<Mesh> ReadMesh(aiMesh* assimpMesh,glm::mat4& MeshTransform);
		void PostProcessScene();
		std::vector<std::shared_ptr<OBJ_Viewer::Material>> GetSceneMaterials(const aiScene* scene);
		std::shared_ptr<OBJ_Viewer::Texture> ReadTexture(aiMaterial* mat, aiTextureType type);
		std::string GetModelTexturePathAbsolute(aiString texturePath)const;
		glm::mat4 AssimpToGlmMatrix4x4(const aiMatrix4x4& matrix);
	private:
		std::vector<aiMesh*> m_meshes;
		std::vector<glm::mat4> m_meshTransforms;
		std::vector<std::shared_ptr<Material>> m_SceneMaterials;
		ModelData m_ModelData;
		std::string m_modelPath;
		LoadModelFileType m_currentlyLoadingType;
		glm::vec3 m_biggestComponents;
		glm::vec3 m_smallestComponents;
		glm::mat4 m_sceneScaleMatrix;
		bool m_AssimpScene;
	};
}

