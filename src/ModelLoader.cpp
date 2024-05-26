#include "ModelLoader.h"
#include <iostream>

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
	Model* result = new Model(meshArray,m_MeshData);
	
	return result;
}

void OBJ_Viewer::ModelLoader::ReadNode(aiNode *node, const aiScene *scene)
{
	for (rsize_t i =0; i < node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
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
	}

	m_MeshData.m_faceCount += assimpMesh->mNumFaces;
	m_MeshData.m_triangleCount += m_MeshData.m_vertexCount / 3;
	for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indexData.push_back(face.mIndices[j]);
	}

	return  std::make_shared<Mesh>(vertexData, indexData,glm::mat4(1),m_SceneMaterials[assimpMesh->mMaterialIndex]);
}
std::vector<std::shared_ptr<OBJ_Viewer::Material>> OBJ_Viewer::ModelLoader::GetSceneMaterials(const aiScene* scene)
{
	std::vector<std::shared_ptr<Material>> result(scene->mNumMaterials);

	for (size_t i =1; i < result.size();i++)
	{
		Material data = {};
		data.m_albedoTexture = ReadTexture(scene->mMaterials[i], aiTextureType_DIFFUSE);
		data.m_ambientOcclusion = ReadTexture(scene->mMaterials[i], aiTextureType_AMBIENT_OCCLUSION);
		data.m_normalTexture = ReadTexture(scene->mMaterials[i], aiTextureType_NORMALS);
		data.m_roughnessTexture = ReadTexture(scene->mMaterials[i], aiTextureType_DIFFUSE_ROUGHNESS);


		result[i] = std::make_shared<Material>(data);
	}

	return result;
}
std::shared_ptr<OBJ_Viewer::Texture> OBJ_Viewer::ModelLoader::ReadTexture(aiMaterial* mat, aiTextureType type)
{
	TextureBuilder builder;
	//TODO:Change to get all of the material textures. A single material can have more than 1 textures in it.
	/*for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{*/
		aiString relativeTexturePath;
		mat->GetTexture(type, 0, &relativeTexturePath);
		//TODO:Check if path is empty if it is than there is no texture and it should continure or return all together;

		std::string fullPath(m_modelPath);
		fullPath.append(relativeTexturePath.C_Str());
		TextureSize textureSize;
		//A way to convert this into a enum of textureFormat.
		int presentChannelCount;
		TexturePixelDataWrapper textureReader(fullPath.c_str(), &textureSize, &presentChannelCount);
		TextureInternalFormat format = TEXTURE_INTERNAL_FORMAT_RGB;
		switch (presentChannelCount)
		{
		case 1:
			format = TEXTURE_INTERNAL_FORMAT_R;
			break;
		case 2:
			format = TEXTURE_INTERNAL_FORMAT_RG;
			break;
		case 3:
			format = TEXTURE_INTERNAL_FORMAT_RGB;
			break;
		}
		return builder.SetTextureSize(textureSize).
			SetTexturePixelData(textureReader.GetTexturePixelData()).
			SetTextureInternalFormat(format).
			SetTextureFormat(TEXTURE_FORMAT_RGB).buildTexture();

	//}
}
