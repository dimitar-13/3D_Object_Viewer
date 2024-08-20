#include "pch.h"
#include "Material.h"


std::weak_ptr<OBJ_Viewer::Texture> OBJ_Viewer::Material::GetMaterialTexture(MaterialTextures_ texture) const
{
	return m_materialInfo.materialTextures.find(texture) != m_materialInfo.materialTextures.end() ? 
		 m_materialInfo.materialTextures.at(texture) : std::weak_ptr<Texture>();
}
