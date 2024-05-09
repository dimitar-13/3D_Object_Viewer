#pragma once
#include<iostream>
namespace OBJ_Viewer
{
	struct ModelData
	{
		uint32_t m_vertexCount;
		uint32_t m_faceCount;
		uint32_t m_triangleCount;
		uint32_t textureCount;
		const char* m_modelPath;
	};
	struct Model
	{
		ModelData m_data;
		//Model matrix.
		//VAO object that holds the data about the VAO,VBO and EBO.
		//Pointer to texture type that handles the texture rendering and the textureUnit id.
	};


	class ModelLoader
	{
	public:
		static Model LoadModel(const char* path);

	};
}

