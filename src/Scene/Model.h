#pragma once
#include"Mesh.h"
#include "pch.h"

namespace OBJ_Viewer {
	struct ModelData
	{
		size_t vertexCount;
		size_t faceCount;
		size_t triangleCount;
		size_t textureCount = 0;
		std::string modelPath = "Unavailable";
	};

	class Model
	{
	public:
		Model(std::vector<std::shared_ptr<Mesh>> meshes,const glm::mat4& modelMatrix, const ModelData& data);
		const ModelData& GetModelData()const { return this->m_data; }
		const std::vector<std::shared_ptr<Mesh>>& GetModelMeshes()const { return this->m_meshes; }
		const glm::mat4& GetModelMatrix()const { return m_ModelMatrix; }
		glm::mat4 GetNormalMatrix()const { return glm::transpose(glm::inverse(m_ModelMatrix)); }
		void GetMatrixDecomposed(glm::vec3& pPosition, glm::vec3& pRotation, glm::vec3& pScale)const;
		void ApplyTransformation(glm::vec3 transform, glm::vec3 scale, glm::vec3 axieToRotate, float rotationAngle);
	private:
		ModelData m_data;
		glm::mat4 m_ModelMatrix;
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}

