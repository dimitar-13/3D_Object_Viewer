#include "pch.h"
#include "ModelLoader.h"
#include "UnitTransformer.h"
#include "Helpers/TextureHelpers.h"
#include "Profiling/AppProfiler.h"


OBJ_Viewer::ModelLoader::ModelLoader(const char* path, LoadModelFileType modelFileType)
{
	Timer timer;

	m_currentlyLoadingType = modelFileType;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate      |
		aiProcess_RemoveRedundantMaterials);

	if (scene == nullptr) 
		return;

	m_biggestComponents = glm::vec3(-std::numeric_limits<float>::min());
	m_smallestComponents = glm::vec3(std::numeric_limits<float>::max());
	m_sceneScaleMatrix = glm::mat4(1);

	m_ModelData.modelPath = std::string(path);

	m_modelPath.append(path);
	m_modelPath = m_modelPath.substr(0, m_modelPath.find_last_of('\\'));
	std::replace(m_modelPath.begin(), m_modelPath.end(), '\\', '/');
	m_modelPath += '/';

	this->ReadNode(scene->mRootNode, scene);
	LoadSceneMaterials(scene);

	auto meshArray = CreateMeshArray();

	PostProcessScene();
	m_loadedScene = new Model(meshArray, AssimpToGlmMatrix4x4(scene->mRootNode->mTransformation) * m_sceneScaleMatrix, m_ModelData);
}

void OBJ_Viewer::ModelLoader::ReadNode(aiNode *node, const aiScene *scene)
{
	glm::mat4 ParentTransform(1);
	for (rsize_t i =0; i < node->mNumMeshes;i++)
	{
		if (node->mParent != nullptr)
			ParentTransform = AssimpToGlmMatrix4x4(node->mParent->mTransformation);

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		m_meshTransforms.push_back(ParentTransform * AssimpToGlmMatrix4x4(node->mTransformation));
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
		result[i] = ReadMesh(m_meshes[i],m_meshTransforms[i]);
	}
	return result;
}

std::shared_ptr<OBJ_Viewer::Mesh >OBJ_Viewer::ModelLoader::ReadMesh(aiMesh* assimpMesh,glm::mat4& MeshTransform)
{
	std::vector<Vertex> vertexData(assimpMesh->mNumVertices);
	m_ModelData.vertexCount += assimpMesh->mNumVertices;
	const glm::mat3 NormalMatrix = glm::mat3(glm::transpose(glm::inverse(MeshTransform)));
	std::vector<unsigned int> indexData;

	for (size_t i = 0; i < assimpMesh->mNumVertices;i++)
	{
		vertexData[i].position.x = assimpMesh->mVertices[i].x;
		vertexData[i].position.y = assimpMesh->mVertices[i].y;
		vertexData[i].position.z = assimpMesh->mVertices[i].z;

		//Pre-bake the transformation into the mesh position data.
		vertexData[i].position = MeshTransform * glm::vec4(vertexData[i].position,1);


		//Record the biggest and smallest components to create a bounding box around the since for post-process scaling.
		m_biggestComponents.x =std::max(m_biggestComponents.x, vertexData[i].position.x);
		m_biggestComponents.y =std::max(m_biggestComponents.y, vertexData[i].position.y);
		m_biggestComponents.z =std::max(m_biggestComponents.z, vertexData[i].position.z);

		m_smallestComponents.x = std::min(m_smallestComponents.x, vertexData[i].position.x);
		m_smallestComponents.y = std::min(m_smallestComponents.y, vertexData[i].position.y);
		m_smallestComponents.z = std::min(m_smallestComponents.z, vertexData[i].position.z);

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
			vertexData[i].normal = NormalMatrix * vertexData[i].normal;
		}
		if (assimpMesh->mTangents != nullptr)
		{
			vertexData[i].tangent.x = assimpMesh->mTangents[i].x;
			vertexData[i].tangent.y = assimpMesh->mTangents[i].y;
			vertexData[i].tangent.z = assimpMesh->mTangents[i].z;
			vertexData[i].tangent = NormalMatrix * vertexData[i].tangent;

		}
	}

	m_ModelData.faceCount += assimpMesh->mNumFaces;
	m_ModelData.triangleCount += m_ModelData.vertexCount / 3;

	for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indexData.push_back(face.mIndices[j]);
	}

	std::unique_ptr<VertexAttributeObject> meshVAO = 
		std::make_unique<VertexAttributeObject>(vertexData, indexData);

	return  std::make_shared<Mesh>(std::move(meshVAO), m_materialRegistry->GetMaterialAtIndex(assimpMesh->mMaterialIndex).lock());
}
void OBJ_Viewer::ModelLoader::PostProcessScene()
{
	/* This method is run after we read all objects from the scene/file. */
#pragma region Adjust scene size
	/* Because we don't want the user to have to guess how to scale their object we "normalize" it and scale it by some
	* app factor. Later I might make the app to work with some APP metric system so that when we import an object in out world/app world the
	* object will be in normal size, internally the object might be scaled by 100 floating point number.
	*/

	//We approximate a vector that covers our scene size. Like a box around out scene
	glm::vec3 scaleDeltas = m_biggestComponents - m_smallestComponents;

	constexpr float APP_SCALING_UNIT = 10.f;
	float scaleFactor = glm::length(scaleDeltas);
	
	//By inversing it we say ok if scaleFactor/(approximated scene size) is small then scale it up if it's too big then scale it down.
	//We also bring the scale factor to range [0:1].
	scaleFactor = 1.0f / scaleFactor;

	m_sceneScaleMatrix = glm::scale(m_sceneScaleMatrix,glm::vec3(APP_SCALING_UNIT*scaleFactor));
#pragma endregion
}

void OBJ_Viewer::ModelLoader::LoadSceneMaterials(const aiScene* scene)
{
	std::vector<std::shared_ptr<Material>> result;
	result.reserve(scene->mNumMaterials);
	for (size_t i =0; i < scene->mNumMaterials;i++)
	{
		
		TypeMaterialRepresentation matRepresentation(m_currentlyLoadingType);
		MaterialData data;
		data.m_materialTextures[MATERIAL_TEXTURE_ALBEDO] = ReadTexture(scene->mMaterials[i], matRepresentation.colorTextureEnum);
		data.m_materialTextures[MATERIAL_TEXTURE_AMBIENT_OCCLUSION] = ReadTexture(scene->mMaterials[i], matRepresentation.ambientOcclusionEnum);
		data.m_materialTextures[MATERIAL_TEXTURE_NORMAL] = ReadTexture(scene->mMaterials[i], matRepresentation.normalTextureEnum);
		data.m_materialTextures[MATERIAL_TEXTURE_ROUGHNESS_METALLIC] = ReadTexture(scene->mMaterials[i], matRepresentation.specularRoughnessEnum);
		result.emplace_back(std::make_shared<Material>(scene->mMaterials[i]->GetName().C_Str(), data));
		//result[i] = std::make_shared<Material>();
	}
	m_materialRegistry = new MaterialRegistry(result);
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
		m_ModelData.textureCount++;

		aiString TexturePath;
		mat->GetTexture(type, i, &TexturePath);
		std::string fullPath = GetModelTexturePathAbsolute(TexturePath);

		TexturePixelReader textureReader(fullPath.c_str());

		TextureFormat format = textureReader.GetTextureFormat();

		//TODO:return vector of textures.
		return builder.SetTextureSize(textureReader.GetTextureSize()).
			SetTexturePixelData(textureReader.GetTexturePixelData()).
			SetTextureInternalFormat(static_cast<TextureInternalFormat>(format)).
			SetTextureFormat(format).buildTexture();
	}
}
