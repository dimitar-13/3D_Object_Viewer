#pragma once
#include<iostream>
#include<glm/glm.hpp>
#include<vector>
#include"Texture.h"
#include"Scene/Material.h"
#include"Scene/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>     // Post processing flags
namespace OBJ_Viewer
{
	class ModelLoader
	{
	public:
		Model* LoadModel(const char* path);
	private:
		void ReadNode(aiNode* node,const aiScene* scene);
		std::vector<std::shared_ptr<Mesh>> CreateMeshArray();
		std::shared_ptr<Mesh> ReadMesh(aiMesh* assimpMesh);
		std::vector<std::shared_ptr<OBJ_Viewer::Material>> GetSceneMaterials(const aiScene* scene);
		std::shared_ptr<OBJ_Viewer::Texture> ReadTexture(aiMaterial* mat, aiTextureType type);
		std::string GetModelTexturePathAbsolute(aiString texturePath)const;
		glm::mat4 AssimpToGlmMatrix4x4(const aiMatrix4x4& matrix);
	private:
		std::vector<aiMesh*> m_meshes;
		std::vector<std::shared_ptr<Material>> m_SceneMaterials;
		ModelData m_MeshData;
		std::string m_modelPath;
		bool m_AssimpScene;
	};
}

