#pragma once
#include"Texture.h"
#include"Rendering/ShaderClass.h"
namespace OBJ_Viewer {
	class Material
	{
	public:
		Material(std::shared_ptr<Texture> albedoTexture,
			std::shared_ptr<Texture> roughnessTexture,
			std::shared_ptr<Texture> normalTexture,
			std::shared_ptr<Texture> ambientOcclusion);
		std::shared_ptr<Texture> GetAlbedoTexture()const { return m_albedoTexture; }
	private:
		//Textures may be shared between materials:One example if 2 models have the same normals but different color texture.
		std::shared_ptr<Texture> m_albedoTexture;
		std::shared_ptr<Texture> m_roughnessTexture;
		std::shared_ptr<Texture> m_normalTexture;
		std::shared_ptr<Texture> m_ambientOcclusion;
	};
}

