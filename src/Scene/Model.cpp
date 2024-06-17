#include "Model.h"
#include<glm/gtc/matrix_transform.hpp>
OBJ_Viewer::Model::Model(std::vector<std::shared_ptr<Mesh>> meshes, glm::mat4 modelMatrix, ModelData data):
	m_ModelMatrix(modelMatrix), m_data(data)
{
	m_meshes = meshes;
}

void OBJ_Viewer::Model::GetMatrixDecomposed(glm::vec3& pPosition, glm::vec3& pRotation, glm::vec3& pScale) const
{
	pPosition.x = this->m_ModelMatrix[3][0];
	pPosition.y = this->m_ModelMatrix[3][1];
	pPosition.z = this->m_ModelMatrix[3][2];

	pScale.x = this->m_ModelMatrix[0][0];
	pScale.y = this->m_ModelMatrix[1][1];
	pScale.z = this->m_ModelMatrix[2][2];
}

void OBJ_Viewer::Model::ApplyTransformation(glm::vec3 transform, glm::vec3 scale, glm::vec3 axisToRotate, float rotationAngle)
{
	m_ModelMatrix = glm::mat4(1);
	this->m_ModelMatrix = glm::translate(this->m_ModelMatrix, transform);
	this->m_ModelMatrix = glm::rotate(this->m_ModelMatrix, rotationAngle, axisToRotate);
	this->m_ModelMatrix = glm::scale(this->m_ModelMatrix, scale);
}
