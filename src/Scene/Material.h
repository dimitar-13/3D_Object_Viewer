#pragma once
#include"Texture.h"
#include"Rendering/ShaderClass.h"
#include <glm/glm.hpp>
namespace OBJ_Viewer {

	//Might be better to favor POD since non of thise members work together;
	struct Material
	{
		glm::vec3 color = { 1,1,1 };
		float roughness = 0.5f;
		std::shared_ptr<Texture> m_albedoTexture;
		std::shared_ptr<Texture> m_roughnessTexture;
		std::shared_ptr<Texture> m_normalTexture;
		std::shared_ptr<Texture> m_ambientOcclusion;
	};

	//class Material
	//{
	//public:
	//	Material(MaterialData data);
	//	std::shared_ptr<Texture> GetAlbedoTexture()const { return m_data.m_albedoTexture; }
	//	glm::vec3 GetColor()const { return m_data.color; }
	//private:
	//	//Textures may be shared between materials:One example if 2 models have the same normals but different color texture.
	//	MaterialData m_data;
	//};
}

