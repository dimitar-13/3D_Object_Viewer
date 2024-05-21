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

	this->ReadNode(scene->mRootNode, scene);
	//Its good if the caller is not in charge of freeing this memory.
	auto meshArray = CreateMeshArray();
	Model* result = new Model(meshArray,m_MeshData);
	
	//TODO:Implement
	return result;
}
//TODO:Interpret the aiMesh data into Mesh format or more specifically method that extracts the vertex data from the aiMesh
//TODO:After we read and construct the meshes create a new Model and make it reference the meshes and return it to the scene.
//TODO:Add a texture read method to read textures.
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
	return  std::make_shared<Mesh>(vertexData, indexData,glm::mat4(1));
}
OBJ_Viewer::Mesh::Mesh(std::vector<OBJ_Viewer::Vertex> vertexData, std::vector<unsigned int>indexData, glm::mat4 transform)
	:m_vao(vertexData, indexData),m_ModelMatrix(transform)
{

}

OBJ_Viewer::Model::Model(std::vector<std::shared_ptr<Mesh>> meshes, ModelData data):m_data(data)
{
	m_meshes = meshes;
	
}

