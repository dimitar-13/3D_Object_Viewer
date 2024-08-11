#include "pch.h"
#include "ModelLoader.h"
#include "UnitTransformer.h"
#include "Helpers/TextureHelpers.h"
#include "Profiling/AppProfiler.h"

OBJ_Viewer::ModelLoader::ModelLoader(const char* modelFilePath, LoadModelFileType_ modelFileType):
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

	auto scene_mesh_array = CreateMeshArray();

	m_ModelData.textureCount = m_materialRegistry->GetRegistryTextureCount();
	m_ModelData.modelPath = std::string(modelFilePath);

	PostProcessScene();
	m_loadedScene = new Model(scene_mesh_array,m_SceneAppNormalizeMatrix, m_ModelData);
}

void OBJ_Viewer::ModelLoader::ReadSceneFile(aiNode* node, const aiScene* scene)
{
	this->ReadNode(scene->mRootNode, scene);
}

void OBJ_Viewer::ModelLoader::ReadNode(aiNode *node, const aiScene *scene)
{
	glm::mat4 mesh_transform_matrix(1);

	for (rsize_t i =0; i < node->mNumMeshes;i++)
	{
		if (node->mParent != nullptr)
			mesh_transform_matrix = AssimpToGlmMatrix4x4(node->mParent->mTransformation* node->mTransformation);

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		m_meshThreadResults.emplace_back(std::async(std::launch::async,
			&ModelLoader::ReadMesh,this, mesh, mesh_transform_matrix));
	}

	for (rsize_t i = 0; i < node->mNumChildren; i++)
	{
		ReadNode(node->mChildren[i], scene);
	}
}


std::unique_ptr<std::vector<OBJ_Viewer::Mesh>> OBJ_Viewer::ModelLoader::CreateMeshArray()
{
	std::unique_ptr<std::vector<Mesh>> mesh_vector_result = std::make_unique<std::vector<Mesh>>();

	mesh_vector_result->reserve(m_meshThreadResults.size());

	for (size_t i = 0; i < m_meshThreadResults.size(); i++)
	{
		const ReadMeshData& mesh_reading_thread_result = m_meshThreadResults[i].get();

		m_sceneBoundingBox.max.x = std::max(mesh_reading_thread_result.mesh_bounding_box.max.x,m_sceneBoundingBox.max.x);
		m_sceneBoundingBox.max.y = std::max(mesh_reading_thread_result.mesh_bounding_box.max.y,m_sceneBoundingBox.max.y);
		m_sceneBoundingBox.max.z = std::max(mesh_reading_thread_result.mesh_bounding_box.max.z,m_sceneBoundingBox.max.z);

		m_sceneBoundingBox.min.x = std::min(mesh_reading_thread_result.mesh_bounding_box.min.x,m_sceneBoundingBox.min.x);
		m_sceneBoundingBox.min.y = std::min(mesh_reading_thread_result.mesh_bounding_box.min.y,m_sceneBoundingBox.min.y);
		m_sceneBoundingBox.min.z = std::min(mesh_reading_thread_result.mesh_bounding_box.min.z,m_sceneBoundingBox.min.z);

		m_ModelData.modelVertexCountData.faceCount += mesh_reading_thread_result.mesh_Information.faceCount;
		m_ModelData.modelVertexCountData.vertexCount += mesh_reading_thread_result.mesh_Information.vertexCount;
		m_ModelData.modelVertexCountData.triangleCount += mesh_reading_thread_result.mesh_Information.triangleCount;

		mesh_vector_result->emplace_back(std::make_unique<VertexAttributeObject>(mesh_reading_thread_result.vertex_data_vector, mesh_reading_thread_result.index_data_vector),
			mesh_reading_thread_result.mesh_material_ID);
	}

	return mesh_vector_result;
}

OBJ_Viewer::ReadMeshData OBJ_Viewer::ModelLoader::ReadMesh(const aiMesh* assimpMesh,const glm::mat4 MeshTransform)
{
	ReadMeshData mesh_data_result;
	mesh_data_result.mesh_material_ID = assimpMesh->mMaterialIndex;

	mesh_data_result.vertex_data_vector.resize(assimpMesh->mNumVertices);

	const glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(MeshTransform)));

	for (size_t i = 0; i < assimpMesh->mNumVertices;i++)
	{
		mesh_data_result.vertex_data_vector[i].position.x = assimpMesh->mVertices[i].x;
		mesh_data_result.vertex_data_vector[i].position.y = assimpMesh->mVertices[i].y;
		mesh_data_result.vertex_data_vector[i].position.z = assimpMesh->mVertices[i].z;

		//Pre-bake the transformation into the mesh position data.
		mesh_data_result.vertex_data_vector[i].position = MeshTransform * glm::vec4(mesh_data_result.vertex_data_vector[i].position,1);

		mesh_data_result.mesh_bounding_box.max.x = std::max(mesh_data_result.mesh_bounding_box.max.x, mesh_data_result.vertex_data_vector[i].position.x);
		mesh_data_result.mesh_bounding_box.max.y = std::max(mesh_data_result.mesh_bounding_box.max.y, mesh_data_result.vertex_data_vector[i].position.y);
		mesh_data_result.mesh_bounding_box.max.z = std::max(mesh_data_result.mesh_bounding_box.max.z, mesh_data_result.vertex_data_vector[i].position.z);

		mesh_data_result.mesh_bounding_box.min.x = std::min(mesh_data_result.mesh_bounding_box.min.x, mesh_data_result.vertex_data_vector[i].position.x);
		mesh_data_result.mesh_bounding_box.min.y = std::min(mesh_data_result.mesh_bounding_box.min.y, mesh_data_result.vertex_data_vector[i].position.y);
		mesh_data_result.mesh_bounding_box.min.z = std::min(mesh_data_result.mesh_bounding_box.min.z, mesh_data_result.vertex_data_vector[i].position.z);

		if (assimpMesh->mTextureCoords[0])
		{
			mesh_data_result.vertex_data_vector[i].uvCoords.x = assimpMesh->mTextureCoords[0][i].x;
			mesh_data_result.vertex_data_vector[i].uvCoords.y = assimpMesh->mTextureCoords[0][i].y;
		}
		else
			mesh_data_result.vertex_data_vector[i].uvCoords = glm::vec2(0.0f, 0.0f);

		if (assimpMesh->mNormals != nullptr)
		{
			mesh_data_result.vertex_data_vector[i].normal.x = assimpMesh->mNormals[i].x;
			mesh_data_result.vertex_data_vector[i].normal.y = assimpMesh->mNormals[i].y;
			mesh_data_result.vertex_data_vector[i].normal.z = assimpMesh->mNormals[i].z;
			mesh_data_result.vertex_data_vector[i].normal = normal_matrix * mesh_data_result.vertex_data_vector[i].normal;
		}
		if (assimpMesh->mTangents != nullptr)
		{
			mesh_data_result.vertex_data_vector[i].tangent.x = assimpMesh->mTangents[i].x;
			mesh_data_result.vertex_data_vector[i].tangent.y = assimpMesh->mTangents[i].y;
			mesh_data_result.vertex_data_vector[i].tangent.z = assimpMesh->mTangents[i].z;
			mesh_data_result.vertex_data_vector[i].tangent = normal_matrix * mesh_data_result.vertex_data_vector[i].tangent;
		}
	}
	constexpr uint8_t kIndexMaxCount = 3;
	const size_t kVectorAllocationSize = static_cast<size_t>(assimpMesh->mNumFaces * 3);

	mesh_data_result.index_data_vector.reserve(kVectorAllocationSize);

	for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			mesh_data_result.index_data_vector.push_back(face.mIndices[j]);
	}
	
	mesh_data_result.mesh_Information.faceCount = assimpMesh->mNumFaces;
	mesh_data_result.mesh_Information.vertexCount = assimpMesh->mNumVertices;
	mesh_data_result.mesh_Information.triangleCount = mesh_data_result.mesh_Information.vertexCount / 3;

	return mesh_data_result;
}

void OBJ_Viewer::ModelLoader::PostProcessScene()
{
	//We approximate a vector that covers our scene size. Like a box around out scene
	glm::vec3 bounding_box_range = m_sceneBoundingBox.max - m_sceneBoundingBox.min;
	glm::vec3 scene_origin = m_sceneBoundingBox.min + (bounding_box_range/2.0f);

	//We use the "-m_sceneBoundingBox.min.y" because we want the scene to be standing above the XZ plane.
	glm::vec3 toOriginTransformVector = glm::vec3{-scene_origin.x , -m_sceneBoundingBox.min.y, -scene_origin.z };

	constexpr float kApplicationScaleUnit = 10.f;
	float scale_factor = glm::length(bounding_box_range);
	
	//By inversing it we say ok if scaleFactor/(approximated scene size) is small then scale it up if it's too big then scale it down.
	//We also bring the scale factor to range [0:1].

	scale_factor = 1.0f / scale_factor;

	const glm::mat4 kTransformSceneToOriginMatrix = glm::translate(glm::mat4(1), toOriginTransformVector);
	m_SceneAppNormalizeMatrix = 
		glm::scale(m_SceneAppNormalizeMatrix,glm::vec3(kApplicationScaleUnit* scale_factor))* kTransformSceneToOriginMatrix;
}

std::vector<std::shared_ptr<OBJ_Viewer::Material>> OBJ_Viewer::ModelLoader::LoadSceneMaterials(const aiScene* scene)
{
	std::vector<std::shared_ptr<Material>> material_vector_result;

	material_vector_result.reserve(scene->mNumMaterials);

	TypeMaterialRepresentation matRepresentation(m_currentlyLoadingType);

	auto AddTextureToMaterialDataIfValid = 
		[](MaterialData& materialData,MaterialTextures_ textureEnumKey, std::shared_ptr<Texture> texture) {
			if (texture)
				materialData.materialTextures[textureEnumKey] = texture;
		};

	for (size_t i =0; i < scene->mNumMaterials;i++)
	{	
		MaterialData data;

		AddTextureToMaterialDataIfValid(data, MaterialTextures_kAlbedo,
			ReadTexture(scene->mMaterials[i], matRepresentation.colorTextureEnum));

		AddTextureToMaterialDataIfValid(data, MaterialTextures_kAmbientOcclusion,
			ReadTexture(scene->mMaterials[i], matRepresentation.ambientOcclusionEnum));

		AddTextureToMaterialDataIfValid(data, MaterialTextures_kNormal,
			ReadTexture(scene->mMaterials[i], matRepresentation.normalTextureEnum));

		AddTextureToMaterialDataIfValid(data, MaterialTextures_kMetalic,
			ReadTexture(scene->mMaterials[i], matRepresentation.specularRoughnessEnum));

		material_vector_result.emplace_back(std::make_shared<Material>(scene->mMaterials[i]->GetName().C_Str(), data));
	}
	return material_vector_result;
}
std::string OBJ_Viewer::ModelLoader::GetTrueTexturePathString(const aiString& texturePath)const
{
	//If textures are not in the same disk location or don't share common path assimp will return absolute path.
	//that's why we need to check for that and return either concatenated path or the absolute path from assimp.

	std::filesystem::path file_system_path(texturePath.C_Str());
	if (file_system_path.is_absolute())
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

	aiString retrieved_texture_path;
	mat->GetTexture(type, 0, &retrieved_texture_path);
	std::string resolved_texture_path = GetTrueTexturePathString(retrieved_texture_path);
	
	TextureBuilder texture_builder;

	TexturePixelReader texture_data_reader (resolved_texture_path.c_str());

	TextureFormat_ texture_format = texture_data_reader.GetTextureFormat();

	return texture_builder.SetTextureSize(texture_data_reader.GetTextureSize()).
			SetTexturePixelData(texture_data_reader.GetTexturePixelData()).
			SetTextureInternalFormat(static_cast<TextureInternalFormat_>(texture_format)).
			SetTextureFormat(texture_format).buildTexture();
	
}
