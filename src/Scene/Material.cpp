#include "Material.h"


std::weak_ptr<OBJ_Viewer::Texture> OBJ_Viewer::Material::GetMaterialTexture(MaterialTextures texture) const
{
	return m_materialInfo.m_materialTextures.find(texture) != m_materialInfo.m_materialTextures.end() ? 
		 m_materialInfo.m_materialTextures.at(texture) : std::weak_ptr<Texture>();
}
