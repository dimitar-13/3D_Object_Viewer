#pragma once
#include"Texture.h"
#include"Rendering/ShaderClass.h"
#include <glm/glm.hpp>
#include <memory>
namespace OBJ_Viewer {

	class TextureStorage
	{
	public:
		TextureStorage();
		std::shared_ptr<Texture> Get1x1WhiteTexture() {return m_1x1WhiteTexture; }
		std::shared_ptr<Texture> Get1x1BlueTexture() { return  m_1x1BlueTexture; }
	private:
		std::shared_ptr<Texture> createTexture(const char* path);
		std::shared_ptr<Texture> m_1x1WhiteTexture;
		std::shared_ptr<Texture> m_1x1BlueTexture;
	};

	struct MaterialData
	{
		std::shared_ptr<Texture> m_albedoTexture;
		std::shared_ptr<Texture> m_roughnessTexture;
		std::shared_ptr<Texture> m_normalTexture;
		std::shared_ptr<Texture> m_ambientOcclusion;
		glm::vec3 color = { 1,1,1 };
		float roughness = 64.f;
	};
	enum MaterialFlags
	{
		IS_ALBEDO_ON = 1,
		IS_CUSTOM_NORMALS_ON = 2,
		IS_AMBIENT_OCCLUSION_ON = 4,
		IS_CUSTOM_SPECULAR_ON = 8
	};
	class Material
	{
	public:
		Material(MaterialData data = {});
		void BindMaterialWithShader(const ShaderClass& shader, MaterialFlags flags)const;
	private:
		MaterialData m_materialInfo;
	};
}

