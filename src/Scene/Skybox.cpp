#include "Skybox.h"
#include"MeshGeneratingMethods.h"
OBJ_Viewer::Skybox::Skybox(std::vector<char*>& textPaths):m_CubeMesh(GenerateCubeModel()->GetModelMeshes().at(0).get())
{
	TextureSize pTextureSize;
	int pPresentChannelsCount;
	glGenTextures(1, &m_cubeMapHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	m_faceTextures.resize(6);
	m_PixelBuffers.resize(6);
	for (size_t i = 0; i < 6; i++)
	{
		TexturePixelDataWrapper reader(textPaths[i], &pTextureSize, &pPresentChannelsCount);
		BufferData pixelBufferData;
		pixelBufferData.type = OPENGL_PIXEL_UNPACK_BUFFER;
		pixelBufferData.data = reader.GetTexturePixelData();
		pixelBufferData.usageType = OPENGL_STREAM_DRAW_BUFFER_USAGE;
		pixelBufferData.bufferSize = pTextureSize.height * pTextureSize.width * pPresentChannelsCount;
		m_PixelBuffers[i] = new OpenGLBuffer(pixelBufferData);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, pTextureSize.width, pTextureSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		TextureBuilder builder;
		m_faceTextures[i] = builder.SetTextureFormat(TEXTURE_FORMAT_RGBA).
			SetTextureInternalFormat(TEXTURE_INTERNAL_FORMAT_RGBA).SetTextureSize(pTextureSize).buildTexture();
	}
	m_CubeMapTextSize = pTextureSize;
	for (size_t i = 0; i < 6; i++)
	{
		m_PixelBuffers[i]->BindBuffer();
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, pTextureSize.height, pTextureSize.width, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		m_faceTextures[i]->BindTexture();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pTextureSize.height, pTextureSize.width, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}

	m_faceTextures[0]->UnbindTexture();
	m_PixelBuffers[0]->UnbindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


}

OBJ_Viewer::Skybox::~Skybox()
{
	for (size_t i = 0; i < m_PixelBuffers.size(); i++)
		delete(m_PixelBuffers[i]);
	glDeleteTextures(1, &this->m_cubeMapHandle);
}

void OBJ_Viewer::Skybox::SwapSkyboxFaceTextures(SkyboxFace toBeSwapped, SkyboxFace swappedWith)
{
	m_PixelBuffers[swappedWith]->BindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + toBeSwapped, 0, 0, 0, m_CubeMapTextSize.height, m_CubeMapTextSize.width, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	m_PixelBuffers[toBeSwapped]->BindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + swappedWith, 0, 0, 0, m_CubeMapTextSize.height, m_CubeMapTextSize.width, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	m_PixelBuffers[0]->UnbindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
