#include "Skybox.h"
#include"MeshGeneratingMethods.h"
#include<algorithm>
OBJ_Viewer::Skybox::Skybox(std::vector<char*>& textPaths):m_skyboxVAO(std::move(GenerateCubeVAO(true)))
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
	TextureFormat format;
	for (size_t i = 0; i < 6; i++)
	{
		TexturePixelDataWrapper reader(textPaths[i], &pTextureSize, &pPresentChannelsCount);
		BufferData pixelBufferData;
		pixelBufferData.type = OPENGL_PIXEL_UNPACK_BUFFER;
		pixelBufferData.data = reader.GetTexturePixelData();
		pixelBufferData.usageType = OPENGL_STREAM_DRAW_BUFFER_USAGE;
		pixelBufferData.bufferSize = pTextureSize.height * pTextureSize.width * pPresentChannelsCount;
		m_PixelBuffers[i] = new OpenGLBuffer(pixelBufferData);
		format = GetFormatByChannelCount(pPresentChannelsCount);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, pTextureSize.width, pTextureSize.height, 0, format, GL_UNSIGNED_BYTE, NULL);

		TextureBuilder builder;
		m_faceTextures[i] = std::move(builder.SetTextureFormat(format).
			SetTextureInternalFormat(static_cast<TextureInternalFormat>(format)).SetTextureSize(pTextureSize).buildTexture());
	}
	m_format = format;
	m_CubeMapTextSize = pTextureSize;
	for (size_t i = 0; i < 6; i++)
	{
		m_PixelBuffers[i]->BindBuffer();
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, pTextureSize.height, pTextureSize.width, m_format, GL_UNSIGNED_BYTE, 0);
		m_faceTextures[i]->BindTexture();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pTextureSize.height, pTextureSize.width, m_format, GL_UNSIGNED_BYTE, 0);
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
	if (toBeSwapped == swappedWith)
		return;

	m_PixelBuffers[swappedWith]->BindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + toBeSwapped, 0, 0, 0, m_CubeMapTextSize.height, m_CubeMapTextSize.width, m_format, GL_UNSIGNED_BYTE, 0);

	m_PixelBuffers[toBeSwapped]->BindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + swappedWith, 0, 0, 0, m_CubeMapTextSize.height, m_CubeMapTextSize.width, m_format, GL_UNSIGNED_BYTE, 0);

	m_PixelBuffers[0]->UnbindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	std::iter_swap(m_faceTextures.begin() + static_cast<int>(toBeSwapped), m_faceTextures.begin() + static_cast<int>(swappedWith));
	std::iter_swap(m_PixelBuffers.begin() + static_cast<int>(toBeSwapped), m_PixelBuffers.begin() + static_cast<int>(swappedWith));

}
