#pragma once
#include<iostream>
#include<glm/glm.hpp>
#include<vector>
#include"VertexAttributeObject.h"
namespace OBJ_Viewer
{
	struct ModelData
	{
		uint32_t m_vertexCount;
		uint32_t m_faceCount;
		uint32_t m_triangleCount;
		uint32_t textureCount;
		const char* m_modelPath;
	};
	class Mesh {
	public:
		//TODO:Change float to Vertex;
		Mesh(std::vector<float> vertexData, std::vector<unsigned int>indexData, glm::mat4 transform);
		const VertexAttributeObject& GetMeshVAO()const { return this->m_vao; }
		const glm::mat4& GetModelMatrix()const { return this->m_ModelMatrix; }
	private:
		glm::mat4 m_ModelMatrix;
		VertexAttributeObject m_vao;
	};
	struct Model
	{
	public:
		Model() {/*TODO:Implment*/ }
		Model(std::vector<Mesh> meshes) :m_meshes(meshes) {/*TODO:Implment*/ }
		const ModelData& GetModelData()const { return this->m_data; }
		const std::vector<Mesh>& GetModelMeshes()const { return this->m_meshes; }
	private:
		ModelData m_data;
		std::vector<Mesh> m_meshes;
	};

	class  ModelLoader
	{
	public:
		static Model* LoadModel(const char* path);

	};
}

