#pragma once
#include"Texture.h"
#include"Rendering/ShaderClass.h"
#include <glm/glm.hpp>
#include <memory>
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
		Material(MaterialData data = {}): m_materialInfo(data)
		{}
		const MaterialData& GetMaterialData()const { return m_materialInfo; }
		std::weak_ptr<Texture> GetMaterialTexture(MaterialTextures texture)const;

	private:
		MaterialData m_materialInfo;
	};
}

