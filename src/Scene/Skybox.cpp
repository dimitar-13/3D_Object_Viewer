#include "Skybox.h"
#include"MeshGeneratingMethods.h"
OBJ_Viewer::Skybox::Skybox(std::array<const char*, 6> textPaths):m_CubeMesh(GenerateCubeModel()->GetModelMeshes().at(0).get())
{
	glGenTextures(1, &m_cubeMapHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	TextureSize pTextureSize;
	int pPresentChannelsCount;
	for (size_t i =0; i < 6;i++)
	{
		TexturePixelDataWrapper reader(textPaths[i], &pTextureSize, &pPresentChannelsCount);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, pTextureSize.width, pTextureSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, reader.GetTexturePixelData());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

OBJ_Viewer::Skybox::~Skybox()
{
	glDeleteTextures(1, &this->m_cubeMapHandle);
}
