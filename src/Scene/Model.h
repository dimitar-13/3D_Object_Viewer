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
		Model(std::vector<std::shared_ptr<Mesh>> meshes,glm::mat4 modelMatrix, ModelData data);
		const ModelData& GetModelData()const { return this->m_data; }
		const std::vector<std::shared_ptr<Mesh>>& GetModelMeshes()const { return this->m_meshes; }
		glm::mat4 GetModelMatrix()const { return m_ModelMatrix; }
		void GetMatrixDecomposed(glm::vec3& pPosition, glm::vec3& pRotation, glm::vec3& pScale)const;
		void ApplyTransformation(glm::vec3 transform, glm::vec3 scale, glm::vec3 axieToRotate, float rotationAngle);
	private:
		ModelData m_data;
		glm::mat4 m_ModelMatrix;
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}

