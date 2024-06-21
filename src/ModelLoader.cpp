#include "ModelLoader.h"
#include <iostream>
#include<filesystem>
#include<glm/gtc/matrix_transform.hpp>

OBJ_Viewer::Model* OBJ_Viewer::ModelLoader::LoadModel(const char* path)
{

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);

	if (nullptr == scene) {
		std::cout << "Assimp failed to load the 3D model file at path:" << path << '\n';
		return nullptr;
	}
	m_modelPath.append(path);
	m_modelPath = m_modelPath.substr(0, m_modelPath.find_last_of('\\'));
	std::replace(m_modelPath.begin(), m_modelPath.end(), '\\', '/');
	m_modelPath += '/';

	this->ReadNode(scene->mRootNode, scene);
	m_SceneMaterials = GetSceneMaterials(scene);

	auto meshArray = CreateMeshArray();
	Model* result = new Model(meshArray, AssimpToGlmMatrix4x4(scene->mRootNode->mTransformation),m_MeshData);
	
	return result;
}

void OBJ_Viewer::ModelLoader::ReadNode(aiNode *node, const aiScene *scene)
{
	
	for (rsize_t i =0; i < node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		node->mTransformation;
		m_meshes.push_back(mesh);
	}
	for (rsize_t i = 0; i < node->mNumChildren; i++)
	{
		ReadNode(node->mChildren[i], scene);
	}
}

std::vector< std::shared_ptr<OBJ_Viewer::Mesh>> OBJ_Viewer::ModelLoader::CreateMeshArray()
{
	std::vector<std::shared_ptr<Mesh>> result(m_meshes.size());

	for (size_t i =0; i < result.size();i++)
	{
		result[i] = ReadMesh(m_meshes[i]);
	}

	return result;
}

std::shared_ptr<OBJ_Viewer::Mesh >OBJ_Viewer::ModelLoader::ReadMesh(aiMesh* assimpMesh)
{
	std::vector<Vertex> vertexData(assimpMesh->mNumVertices);
	m_MeshData.m_vertexCount += assimpMesh->mNumVertices;
	std::vector<unsigned int> indexData;
	for (size_t i = 0; i < assimpMesh->mNumVertices;i++)
	{
		vertexData[i].position.x = assimpMesh->mVertices[i].x;
		vertexData[i].position.y = assimpMesh->mVertices[i].y;
		vertexData[i].position.z = assimpMesh->mVertices[i].z;

		if (assimpMesh->mTextureCoords[0])
		{
			vertexData[i].uvCoords.x = assimpMesh->mTextureCoords[0][i].x;
			vertexData[i].uvCoords.y = assimpMesh->mTextureCoords[0][i].y;
		}
		else
			vertexData[i].uvCoords = glm::vec2(0.0f, 0.0f);
		if (assimpMesh->mNormals != nullptr)
		{
			vertexData[i].normal.x = assimpMesh->mNormals[i].x;
			vertexData[i].normal.y = assimpMesh->mNormals[i].y;
			vertexData[i].normal.z = assimpMesh->mNormals[i].z;	
		}
		if (assimpMesh->mTangents != nullptr)
		{
			vertexData[i].tangent.x = assimpMesh->mTangents[i].x;
			vertexData[i].tangent.y = assimpMesh->mTangents[i].y;
			vertexData[i].tangent.z = assimpMesh->mTangents[i].z;
		}
	}

	m_MeshData.m_faceCount += assimpMesh->mNumFaces;
	m_MeshData.m_triangleCount += m_MeshData.m_vertexCount / 3;
	for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indexData.push_back(face.mIndices[j]);
	}
	std::unique_ptr<VertexAttributeObject> meshVAO = 
		std::make_unique<VertexAttributeObject>(vertexData, indexData);

	return  std::make_shared<Mesh>(std::move(meshVAO), m_SceneMaterials[assimpMesh->mMaterialIndex]);
}
std::vector<std::shared_ptr<OBJ_Viewer::Material>> OBJ_Viewer::ModelLoader::GetSceneMaterials(const aiScene* scene)
{
	std::vector<std::shared_ptr<Material>> result(scene->mNumMaterials);

	for (size_t i =0; i < result.size();i++)
	{
		
		MaterialData data;
		data.m_albedoTexture = ReadTexture(scene->mMaterials[i], aiTextureType_DIFFUSE);
		data.m_ambientOcclusion = ReadTexture(scene->mMaterials[i], aiTextureType_AMBIENT_OCCLUSION);
		data.m_normalTexture = ReadTexture(scene->mMaterials[i], aiTextureType_HEIGHT);
		data.m_roughnessTexture = ReadTexture(scene->mMaterials[i], aiTextureType_UNKNOWN);

		result[i] = std::make_shared<Material>(data);
	}

	return result;
}
std::string OBJ_Viewer::ModelLoader::GetModelTexturePathAbsolute(aiString texturePath)const
{
	//If textures are not in the same disk location or don't share common path assimp will return absolute path.
	//that's why we need to check for that and return either concatenated path or the absolute path from assimp.

	std::filesystem::path fileSysPath(texturePath.C_Str());
	if (fileSysPath.is_absolute())
		return std::string(texturePath.C_Str());

	return std::string(m_modelPath + texturePath.C_Str());
}
glm::mat4 OBJ_Viewer::ModelLoader::AssimpToGlmMatrix4x4(const aiMatrix4x4& matrix)
{
	if (matrix.IsIdentity())
		return glm::mat4(1);

	glm::mat4 result(1);
	aiVector3D Position;
	aiVector3D Rotation;
	aiVector3D Scale;
	ai_real RotationAngle;
	matrix.Decompose(Scale,Rotation, RotationAngle,Position);
	
	result = glm::translate(result, glm::vec3(Position.x, Position.y, Position.z));
	result = glm::rotate(result, RotationAngle, glm::vec3(Rotation.x, Rotation.y, Rotation.z));
	result = glm::scale(result, glm::vec3(Scale.x, Scale.y, Scale.z));

	return result;
}
std::shared_ptr<OBJ_Viewer::Texture> OBJ_Viewer::ModelLoader::ReadTexture(aiMaterial* mat, aiTextureType type)
{
	uint32_t textureCount = mat->GetTextureCount(type);
	if (textureCount == 0)
		return nullptr;

	TextureBuilder builder;
	//TODO:Change to get all of the material textures. A single material can have more than 1 textures in it.
	for (uint32_t i = 0; i < textureCount; i++)
	{
		aiString TexturePath;
		mat->GetTexture(type, i, &TexturePath);
		std::string fullPath = GetModelTexturePathAbsolute(TexturePath);

		TextureSize textureSize;
		int presentChannelCount;

		TexturePixelDataWrapper textureReader(fullPath.c_str(), &textureSize, &presentChannelCount);

		TextureFormat format = GetFormatByChannelCount(presentChannelCount);

		//TODO:return vector of textures.
		return builder.SetTextureSize(textureSize).
			SetTexturePixelData(textureReader.GetTexturePixelData()).
			SetTextureInternalFormat(static_cast<TextureInternalFormat>(format)).
			SetTextureFormat(format).buildTexture();
	}
}
