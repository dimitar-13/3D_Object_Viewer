#pragma once
#include "pch.h"
#include "Material.h"
namespace OBJ_Viewer {

	class MaterialRegistry
	{
	public:
		MaterialRegistry(const std::vector<std::shared_ptr<Material>>& sceneMaterials):
			m_materialHash(sceneMaterials)
		{}

		std::weak_ptr<Material> GetMaterialAtIndex(const size_t i) { assert(!(i >= m_materialHash.size())); return std::weak_ptr<Material>(m_materialHash.at(i)); }
		const std::weak_ptr<Material> GetMaterialAtIndex(const size_t i)const { assert(!(i >= m_materialHash.size())); return std::weak_ptr<Material>(m_materialHash.at(i)); }

		size_t GetRegistrySize()const { return m_materialHash.size(); }
	private:
		std::vector<std::shared_ptr<Material>> m_materialHash;
	};
}

