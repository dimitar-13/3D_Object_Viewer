#pragma once
#include<iostream>
#include<glm/glm.hpp>
#include<vector>
#include"VertexAttributeObject.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>     // Post processing flags
namespace OBJ_Viewer
{
	struct ModelData
	{
		uint32_t m_vertexCount;
		uint32_t m_faceCount;
		uint32_t m_triangleCount;
		uint32_t textureCount;
		//const char* m_modelPath;
	};
	class Mesh {
	public:
		Mesh(std::vector<OBJ_Viewer::Vertex> vertexData, std::vector<unsigned int>indexData, glm::mat4 transform);
		const VertexAttributeObject& GetMeshVAO()const { return this->m_vao; }
		const glm::mat4& GetModelMatrix()const { return this->m_ModelMatrix; }
	private:
		glm::mat4 m_ModelMatrix;
		VertexAttributeObject m_vao;
	};
	struct Model
	{
	public:
		Model(std::vector<std::shared_ptr<Mesh>> meshes, ModelData data);
		const ModelData& GetModelData()const { return this->m_data; }
		const std::vector<std::shared_ptr<Mesh>>& GetModelMeshes()const { return this->m_meshes; }
	private:
		ModelData m_data;
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};

	class ModelLoader
	{
	public:
		Model* LoadModel(const char* path);
	private:
		void ReadNode(aiNode* node,const aiScene* scene);
		std::vector<std::shared_ptr<Mesh>> CreateMeshArray();
		std::shared_ptr<Mesh> ReadMesh(aiMesh* assimpMesh);
		void ReadTexture();
	private:
		std::vector<aiMesh*> m_meshes;
		ModelData m_MeshData;
		bool m_AssimpScene;
	};
}

