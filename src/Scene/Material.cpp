#include "Material.h"
OBJ_Viewer::Material::Material(std::shared_ptr<Texture> albedoTexture,
	std::shared_ptr<Texture> roughnessTexture,
	std::shared_ptr<Texture> normalTexture,
	std::shared_ptr<Texture> ambientOcclusion)
{
	this->m_albedoTexture = albedoTexture;
	this->m_roughnessTexture = roughnessTexture;
	this->m_normalTexture = normalTexture;
	this->m_ambientOcclusion = ambientOcclusion;
}
