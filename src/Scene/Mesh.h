#pragma once
#include "gpu_side/VertexAttributeObject.h"
#include "Material.h"
#include "pch.h"

namespace OBJ_Viewer
{
    /**
     * @brief Class representing a 3D mesh.
     *
     * This class encapsulates the concept of a 3D mesh, which includes a vertex attribute object used for rendering and
     * an optional material index. The `VertexAttributeObject` is used to manage the vertex and index data for rendering
     * the mesh. The material index is used to retrieve the associated material from the `MaterialRegistry`, which defines
     * the appearance of the mesh.
     *
     * The `Mesh` class enables the creation and manipulation of 3D mesh objects, including accessing their rendering data
     * and associated materials.
     *
     * @see VertexAttributeObject for details on the vertex attribute object.
     * @see MaterialRegistry for details on material management.
     */
	class Mesh {
	public:
        /**
         * @brief Constructor for creating a `Mesh` object.
         *
         * This constructor initializes a `Mesh` with the provided vertex attribute object and optionally assigns
         * a material index. The `VertexAttributeObject` should be a valid object representing the vertex data and
         * attributes necessary for rendering the mesh. If no material index is provided, it defaults to `0`.
         *
         * @param meshVAo A unique pointer to a `VertexAttributeObject` that defines the vertex attributes and data for the mesh.
         * @param material_index An optional index specifying the material associated with this mesh. Defaults to `0` if not provided.
         */
		Mesh(std::unique_ptr<VertexAttributeObject> meshVAo, size_t material_index = 0);
        /**
         * @brief Retrieves a constant reference to the mesh's `VertexAttributeObject`.
         *
         * This method provides access to the `VertexAttributeObject` associated with the mesh. It returns a constant reference,
         * allowing read-only access to the vertex attributes and data used for rendering the mesh. This ensures that the `VertexAttributeObject`
         * cannot be modified through this reference.
         *
         * @returns A constant reference to the `VertexAttributeObject` of the mesh.
         */
		const VertexAttributeObject& GetMeshVAO()const { return *this->m_vao; }
        /**
         * @brief Retrieves the material index associated with the mesh.
         *
         * This method returns the index of the material assigned to the mesh. The index can be used to look up the material
         * in the `MaterialRegistry`, allowing you to access material properties and textures.
         *
         * @returns The material index of the mesh.
         */
        size_t GetMaterialIndex()const { return m_meshRegistryMaterialIndex; }
	private:
        size_t m_meshRegistryMaterialIndex;
		std::unique_ptr<VertexAttributeObject> m_vao;
	};
}

