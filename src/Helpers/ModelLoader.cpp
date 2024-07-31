#include "pch.h"
#include "ModelLoader.h"
#include "UnitTransformer.h"
#include "Helpers/TextureHelpers.h"
#include "Profiling/AppProfiler.h"

OBJ_Viewer::ModelLoader::ModelLoader(const char* modelFilePath, LoadModelFileType modelFileType):
	m_currentlyLoadingType(modelFileType)
{
	Timer timer;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFilePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate      |
		aiProcess_RemoveRedundantMaterials);
		
	if (scene == nullptr)
	{
		LOGGER_LOG_ERROR("Assimp failed to load file at path:{0} \n Error message:{1}", modelFilePath, importer.GetErrorString());
		return;
	}
	auto GetPathNoFilename = [](std::string& path)
		{
			path.substr(0, path.find_last_of('\\') + 1);
			std::replace(path.begin(), path.end(), '\\', '/');
		};

	m_modelPath = modelFilePath;
	GetPathNoFilename(m_modelPath);

	m_materialRegistry = new MaterialRegistry(LoadSceneMaterials(scene));

	ReadSceneFile(scene->mRootNode, scene);

	auto meshArray = CreateMeshArray();

	m_ModelData.textureCount = m_materialRegistry->GetRegistryTextureCount();
	m_ModelData.modelPath = std::string(modelFilePath);

	PostProcessScene();
	m_loadedScene = new Model(meshArray,m_SceneAppNormalizeMatrix, m_ModelData);
}

void OBJ_Viewer::ModelLoader::ReadSceneFile(aiNode* node, const aiScene* scene)
{
	this->ReadNode(scene->mRootNode, scene);
}

void OBJ_Viewer::ModelLoader::ReadNode(aiNode *node, const aiScene *scene)
{
	glm::mat4 transformMatrix(1);

	for (rsize_t i =0; i < node->mNumMeshes;i++)
	{
		if (node->mParent != nullptr)
			transformMatrix = AssimpToGlmMatrix4x4(node->mParent->mTransformation* node->mTransformation);

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		m_meshThreadResults.emplace_back(std::async(std::launch::async,
			&ModelLoader::ReadMesh,this, mesh, transformMatrix));
	}

	for (rsize_t i = 0; i < node->mNumChildren; i++)
	{
		ReadNode(node->mChildren[i], scene);
	}
}


std::unique_ptr<std::vector<OBJ_Viewer::Mesh>> OBJ_Viewer::ModelLoader::CreateMeshArray()
{
	std::unique_ptr<std::vector<Mesh>> result = std::make_unique<std::vector<Mesh>>();
	result->reserve(m_meshThreadResults.size());

	for (size_t i = 0; i < m_meshThreadResults.size(); i++)
	{
		const ReadMeshData& threadResult = m_meshThreadResults[i].get();

		m_sceneBoundingBox.max.x = std::max(threadResult.meshBoundingBox.max.x,m_sceneBoundingBox.max.x);
		m_sceneBoundingBox.max.y = std::max(threadResult.meshBoundingBox.max.y,m_sceneBoundingBox.max.y);
		m_sceneBoundingBox.max.z = std::max(threadResult.meshBoundingBox.max.z,m_sceneBoundingBox.max.z);

		m_sceneBoundingBox.min.x = std::min(threadResult.meshBoundingBox.min.x,m_sceneBoundingBox.min.x);
		m_sceneBoundingBox.min.y = std::min(threadResult.meshBoundingBox.min.y,m_sceneBoundingBox.min.y);
		m_sceneBoundingBox.min.z = std::min(threadResult.meshBoundingBox.min.z,m_sceneBoundingBox.min.z);

		m_ModelData.meshInfo.faceCount += threadResult.meshInfo.faceCount;
		m_ModelData.meshInfo.vertexCount += threadResult.meshInfo.vertexCount;
		m_ModelData.meshInfo.triangleCount += threadResult.meshInfo.triangleCount;

		result->emplace_back(std::make_unique<VertexAttributeObject>(threadResult.vertexData, threadResult.indexData),
			m_materialRegistry->GetMaterialAtIndex(threadResult.meshMaterialID).lock());
	}

	return result;
}

OBJ_Viewer::ReadMeshData OBJ_Viewer::ModelLoader::ReadMesh(const aiMesh* assimpMesh,const glm::mat4 MeshTransform)
{
	ReadMeshData result;
	result.meshMaterialID = assimpMesh->mMaterialIndex;

	result.vertexData.resize(assimpMesh->mNumVertices);

	const glm::mat3 NormalMatrix = glm::mat3(glm::transpose(glm::inverse(MeshTransform)));

	for (size_t i = 0; i < assimpMesh->mNumVertices;i++)
	{
		result.vertexData[i].position.x = assimpMesh->mVertices[i].x;
		result.vertexData[i].position.y = assimpMesh->mVertices[i].y;
		result.vertexData[i].position.z = assimpMesh->mVertices[i].z;

		//Pre-bake the transformation into the mesh position data.
		result.vertexData[i].position = MeshTransform * glm::vec4(result.vertexData[i].position,1);

		result.meshBoundingBox.max.x = std::max(result.meshBoundingBox.max.x, result.vertexData[i].position.x);
		result.meshBoundingBox.max.y = std::max(result.meshBoundingBox.max.y, result.vertexData[i].position.y);
		result.meshBoundingBox.max.z = std::max(result.meshBoundingBox.max.z, result.vertexData[i].position.z);

		result.meshBoundingBox.min.x = std::min(result.meshBoundingBox.min.x, result.vertexData[i].position.x);
		result.meshBoundingBox.min.y = std::min(result.meshBoundingBox.min.y, result.vertexData[i].position.y);
		result.meshBoundingBox.min.z = std::min(result.meshBoundingBox.min.z, result.vertexData[i].position.z);

		if (assimpMesh->mTextureCoords[0])
		{
			result.vertexData[i].uvCoords.x = assimpMesh->mTextureCoords[0][i].x;
			result.vertexData[i].uvCoords.y = assimpMesh->mTextureCoords[0][i].y;
		}
		else
			result.vertexData[i].uvCoords = glm::vec2(0.0f, 0.0f);

		if (assimpMesh->mNormals != nullptr)
		{
			result.vertexData[i].normal.x = assimpMesh->mNormals[i].x;
			result.vertexData[i].normal.y = assimpMesh->mNormals[i].y;
			result.vertexData[i].normal.z = assimpMesh->mNormals[i].z;
			result.vertexData[i].normal = NormalMatrix * result.vertexData[i].normal;
		}
		if (assimpMesh->mTangents != nullptr)
		{
			result.vertexData[i].tangent.x = assimpMesh->mTangents[i].x;
			result.vertexData[i].tangent.y = assimpMesh->mTangents[i].y;
			result.vertexData[i].tangent.z = assimpMesh->mTangents[i].z;
			result.vertexData[i].tangent = NormalMatrix * result.vertexData[i].tangent;
		}
	}
	constexpr uint8_t INDEX_MAX_COUNT = 3;
	const size_t VECTOR_ALLOCATION_SIZE = static_cast<size_t>(assimpMesh->mNumFaces * 3);

	result.indexData.reserve(VECTOR_ALLOCATION_SIZE);

	for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			result.indexData.push_back(face.mIndices[j]);
	}
	
	result.meshInfo.faceCount = assimpMesh->mNumFaces;
	result.meshInfo.vertexCount = assimpMesh->mNumVertices;
	result.meshInfo.triangleCount = result.meshInfo.vertexCount / 3;

	return result;
}

void OBJ_Viewer::ModelLoader::PostProcessScene()
{
	//We approximate a vector that covers our scene size. Like a box around out scene
	glm::vec3 boundingBoxRange = m_sceneBoundingBox.max - m_sceneBoundingBox.min;
	glm::vec3 SceneOrigin = m_sceneBoundingBox.min + (boundingBoxRange/2.0f);

	//We use the "-m_sceneBoundingBox.min.y" because we want the scene to be standing above the XZ plane.
	glm::vec3 toOriginTransformVector = glm::vec3{-SceneOrigin.x , -m_sceneBoundingBox.min.y, -SceneOrigin.z };

	constexpr float APP_SCALING_UNIT = 10.f;
	float scaleFactor = glm::length(boundingBoxRange);
	
	//By inversing it we say ok if scaleFactor/(approximated scene size) is small then scale it up if it's too big then scale it down.
	//We also bring the scale factor to range [0:1].
	scaleFactor = 1.0f / scaleFactor;
	const glm::mat4 TO_ORIGIN_TRANSFORM = glm::translate(glm::mat4(1), toOriginTransformVector);
	m_SceneAppNormalizeMatrix = glm::scale(m_SceneAppNormalizeMatrix,glm::vec3(APP_SCALING_UNIT*scaleFactor))* TO_ORIGIN_TRANSFORM;
}

std::vector<std::shared_ptr<OBJ_Viewer::Material>> OBJ_Viewer::ModelLoader::LoadSceneMaterials(const aiScene* scene)
{
	std::vector<std::shared_ptr<Material>> result;

	result.reserve(scene->mNumMaterials);

	TypeMaterialRepresentation matRepresentation(m_currentlyLoadingType);

	auto AddTextureToMaterialDataIfValid = 
		[](MaterialData& materialData,MaterialTextures textureEnumKey, std::shared_ptr<Texture> texture) {
			if (texture)
				materialData.m_materialTextures[textureEnumKey] = texture;
		};

	for (size_t i =0; i < scene->mNumMaterials;i++)
	{	
		MaterialData data;

		AddTextureToMaterialDataIfValid(data, MATERIAL_TEXTURE_ALBEDO,
			ReadTexture(scene->mMaterials[i], matRepresentation.colorTextureEnum));

		AddTextureToMaterialDataIfValid(data, MATERIAL_TEXTURE_AMBIENT_OCCLUSION,
			ReadTexture(scene->mMaterials[i], matRepresentation.ambientOcclusionEnum));

		AddTextureToMaterialDataIfValid(data, MATERIAL_TEXTURE_NORMAL,
			ReadTexture(scene->mMaterials[i], matRepresentation.normalTextureEnum));

		AddTextureToMaterialDataIfValid(data, MATERIAL_TEXTURE_ROUGHNESS_METALLIC,
			ReadTexture(scene->mMaterials[i], matRepresentation.specularRoughnessEnum));

		result.emplace_back(std::make_shared<Material>(scene->mMaterials[i]->GetName().C_Str(), data));
	}
	return result;
}
std::string OBJ_Viewer::ModelLoader::GetTrueTexturePathString(const aiString& texturePath)const
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
	if (mat->GetTextureCount(type) == 0)
		return std::shared_ptr<Texture>();

	aiString retrievedTexturePath;
	mat->GetTexture(type, 0, &retrievedTexturePath);
	std::string resolvedTexturePath = GetTrueTexturePathString(retrievedTexturePath);
	
	TextureBuilder textureBuilder;

	TexturePixelReader textureReader(resolvedTexturePath.c_str());

	TextureFormat format = textureReader.GetTextureFormat();

	return textureBuilder.SetTextureSize(textureReader.GetTextureSize()).
			SetTexturePixelData(textureReader.GetTexturePixelData()).
			SetTextureInternalFormat(static_cast<TextureInternalFormat>(format)).
			SetTextureFormat(format).buildTexture();
	
}
