#pragma once
#include<iostream>
#include"Mesh.h"
namespace OBJ_Viewer {
	struct ModelData
	{
		uint32_t m_vertexCount;
		uint32_t m_faceCount;
		uint32_t m_triangleCount;
		uint32_t textureCount;
		//const char* m_modelPath;
	};

	class Model
	{
	public:
		Model(std::vector<std::shared_ptr<Mesh>> meshes, ModelData data);
		const ModelData& GetModelData()const { return this->m_data; }
		const std::vector<std::shared_ptr<Mesh>>& GetModelMeshes()const { return this->m_meshes; }
	private:
		ModelData m_data;
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}

