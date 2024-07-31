#pragma once
#include "gpu_side/Texture.h"
#include "gpu_side/ShaderClass.h"
#include "pch.h"

namespace OBJ_Viewer {
	enum MaterialTextures
	{
		MATERIAL_TEXTURE_UNKNOWN,
		MATERIAL_TEXTURE_ALBEDO,
		MATERIAL_TEXTURE_NORMAL,
		MATERIAL_TEXTURE_ROUGHNESS_METALLIC,
		MATERIAL_TEXTURE_AMBIENT_OCCLUSION
	};

	struct MaterialData
	{
		std::unordered_map< MaterialTextures, std::shared_ptr<Texture>> m_materialTextures;
		glm::vec3 color = { 1,1,1 };
		float roughness = 64.f;
	};
	enum MaterialFlags
	{
		FLAGS_NONE = 0,
		IS_ALBEDO_ON = 1,
		IS_CUSTOM_NORMALS_ON = 2,
		IS_AMBIENT_OCCLUSION_ON = 4,
		IS_CUSTOM_SPECULAR_ON = 8,
		FLAGS_ALL = IS_ALBEDO_ON | IS_CUSTOM_NORMALS_ON | IS_AMBIENT_OCCLUSION_ON | IS_CUSTOM_SPECULAR_ON
	};
	class Material
	{
	public:
		Material(const std::string& materialName, MaterialData data = {}):
			m_materialInfo(data),m_materialName(materialName)
		{}
		size_t GetMaterialTextureCount()const { return m_materialInfo.m_materialTextures.size(); }
		glm::vec3 GetMaterialBaseColor()const { return m_materialInfo.color; }
		float GetMaterialRoughness()const { return m_materialInfo.roughness; }
		std::weak_ptr<Texture> GetMaterialTexture(MaterialTextures texture)const;
		const std::string& GetMaterialName()const { return m_materialName; }
	private:
		MaterialData m_materialInfo;
		std::string m_materialName;
	};
}

