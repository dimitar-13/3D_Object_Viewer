#pragma once
#include "gpu_side/Texture.h"
#include "gpu_side/ShaderClass.h"
#include "pch.h"
#include "Enums/MaterialFlags.h"
namespace OBJ_Viewer {

	struct MaterialData
	{
		std::unordered_map< MaterialTextures_, std::shared_ptr<Texture>> m_materialTextures;
		glm::vec3 color = { 1,1,1 };
		float roughness = 64.f;
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
		std::weak_ptr<Texture> GetMaterialTexture(MaterialTextures_ texture)const;
		const std::string& GetMaterialName()const { return m_materialName; }
	private:
		MaterialData m_materialInfo;
		std::string m_materialName;
	};
}

