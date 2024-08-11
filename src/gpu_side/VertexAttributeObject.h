#pragma once
#include "pch.h"
namespace OBJ_Viewer
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uvCoords;
		glm::vec3 normal;
		glm::vec3 tangent;
		//TODO:Add more
	};
	struct VertexAttribData
	{
		size_t indexCount;
		size_t vertexCount;
	};
    /**
     * @brief Class for creating and managing a GPU-based Vertex Attribute Object (VAO).
     *
     * This class is responsible for creating and managing a Vertex Attribute Object (VAO) in GPU memory.
     * It handles the creation and management of both vertex and index arrays, providing functionality to
     * access the vertex and index counts. The class is designed to work with both a `Vertex` data structure
     * and a position array. Additionally, the class allows for binding and unbinding the VAO to the OpenGL context.
     *
     * The `VertexAttributeObject` class simplifies VAO management, ensuring that vertex and index data
     * are efficiently handled within the GPU.
     *
     * @note Currently, this class is non-copyable due to the low demand for object duplication.
     *       If such a need arises in the future, this restriction may be reconsidered.
     */
	class VertexAttributeObject
	{
	public:
		using IndexDataType = unsigned int;
        /**
         * @brief Constructor for creating a Vertex Attribute Object (VAO) using vertex and index data.
         *
         * This constructor initializes a Vertex Attribute Object (VAO) along with a vertex buffer and an index buffer
         * using the provided arrays of vertices and indices. The VAO is set up to store the relationship between the vertex
         * data and the attributes in the shader, while the buffers hold the actual vertex and index data in GPU memory.
         * It also stores the vertex and index count.
         *
         * @param kVertexDataVectorRef A reference(temporary) to an array of vertices.
         * @param kIndexDataVectorRef A reference(temporary) to an array of indices.
         */
		VertexAttributeObject(const std::vector<Vertex>& kVertexDataVectorRef,const std::vector<IndexDataType>& kIndexDataVectorRef);
        /**
         * @brief Constructor for creating a Vertex Attribute Object (VAO) using 3D position and index data.
         *
         * This constructor initializes a Vertex Attribute Object (VAO) along with a vertex buffer and an index buffer
         * using the provided arrays of 3D positions and indices. The VAO is set up to store the relationship between
         * the vertex position data and the attributes in the shader, while the buffers store the actual vertex and
         * index data in GPU memory. Additionally, it tracks the count of vertices and indices for later use.
         *
         * @param kPostionDataVectorRef A reference(temporary) to an array of 3D position vertices.
         * @param kIndexDataVectorRef A reference(temporary) to an array of indices.
         */
		VertexAttributeObject(const std::vector<glm::vec3>& kPostionDataVectorRef, const std::vector<IndexDataType>& kIndexDataVectorRef);
        /**
         * @brief Retrieves the index count of the vertex attribute object. 
        */
		size_t GetIndexCount()const { return this->m_vertexData.indexCount; }
        /**
         * @brief Retrieves the vertex count of the vertex attribute object.
        */
		size_t GetVertexCount()const { return this->m_vertexData.vertexCount; }
        /**
         * @brief Binds the Vertex Attribute Object (VAO).
         *
         * This method binds the Vertex Attribute Object (VAO) to the OpenGL context. Once bound,
         * all subsequent operations such as drawing commands will utilize this VAO. Binding a VAO
         * ensures that the correct vertex data and attribute configurations are used in rendering.
         */
		void BindBuffer()const{ glBindVertexArray(this->m_vertexAttributeObjHandle); }
        /**
         * @brief Unbinds the vertex attribute object. 
        */
		void UnBind()const { glBindVertexArray(0); }
        /**
         * @brief Deletes the Vertex attribute object(VAO) along side every other resource.
         *
         * Deletes the VAO, vertex buffer and index buffer.
        */
        ~VertexAttributeObject();
	private:
		GLuint m_vertexBufferObjHandle;
		GLuint m_vertexAttributeObjHandle;
		GLuint m_indexBufferObjHandle;
		VertexAttribData m_vertexData;
	};
}

