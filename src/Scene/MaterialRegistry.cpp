#include "pch.h"
#include "MaterialRegistry.h"

OBJ_Viewer::MaterialRegistry::MaterialRegistry(const std::vector<std::shared_ptr<Material>>& sceneMaterials):
	m_materialHash(sceneMaterials)
{
	for (const auto& material : sceneMaterials)
	{
		m_textureCount += material->GetMaterialTextureCount();
	}
}
