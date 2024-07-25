#include "pch.h"
#include "Skybox.h"
#include "Helpers/MeshGeneratingMethods.h"

OBJ_Viewer::Skybox::Skybox(const std::array<TexturePixelReader, Skybox::SKYBOX_FACE_COUNT>& textures):
	m_skyboxVAO(GenerateCubeVAO(true))
{
	const Size2D SkyboxTextureSize = textures[0].GetTextureSize();

	const size_t SKYBOX_FACE_TEXTURE_BUFFER_BYTE_SIZE = static_cast<size_t>(SkyboxTextureSize.height)
		* static_cast<size_t>(SkyboxTextureSize.width) * textures[0].GetChannelCount();

	BufferData pixelBufferData;
	pixelBufferData.type = OPENGL_PIXEL_UNPACK_BUFFER;
	pixelBufferData.usageType = OPENGL_STREAM_DRAW_BUFFER_USAGE;
	pixelBufferData.bufferSize = SKYBOX_FACE_TEXTURE_BUFFER_BYTE_SIZE;

	m_CubeMapTextSize = SkyboxTextureSize;
	m_format = textures[0].GetTextureFormat();

#pragma region GPU side skybox settings
	glGenTextures(1, &m_cubeMapHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#pragma endregion
	
	m_faceTextures.reserve(SKYBOX_FACE_COUNT);

	m_PixelBuffers.reserve(SKYBOX_FACE_COUNT);

	for (size_t i = 0; i < SKYBOX_FACE_COUNT; i++)
	{	
		pixelBufferData.data = textures[i].GetTexturePixelData();

		m_PixelBuffers.emplace_back(pixelBufferData);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_format, SkyboxTextureSize.width, SkyboxTextureSize.height,
			0, m_format, GL_UNSIGNED_BYTE, textures[i].GetTexturePixelData());

		TextureBuilder builder;

		m_faceTextures.emplace_back(builder.SetTextureFormat(m_format).
			SetTextureInternalFormat(static_cast<TextureInternalFormat>(m_format))
			.SetTextureSize(SkyboxTextureSize).SetTexturePixelData(textures[i].GetTexturePixelData()).buildTexture());
	}

	m_faceTextures[0]->UnbindTexture();
	m_PixelBuffers[0].UnbindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OBJ_Viewer::Skybox::~Skybox()
{
	glDeleteTextures(1, &this->m_cubeMapHandle);
}

void OBJ_Viewer::Skybox::SwapSkyboxFaceTextures(SkyboxFace toBeSwapped, SkyboxFace swappedWith)
{
	if (toBeSwapped == swappedWith)
		return;

	m_PixelBuffers[swappedWith].BindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + toBeSwapped, 0, 0, 0, m_CubeMapTextSize.height, m_CubeMapTextSize.width, m_format, GL_UNSIGNED_BYTE, 0);

	m_PixelBuffers[toBeSwapped].BindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + swappedWith, 0, 0, 0, m_CubeMapTextSize.height, m_CubeMapTextSize.width, m_format, GL_UNSIGNED_BYTE, 0);

	m_PixelBuffers[0].UnbindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	std::iter_swap(m_faceTextures.begin() + static_cast<int>(toBeSwapped), m_faceTextures.begin() + static_cast<int>(swappedWith));
	std::iter_swap(m_PixelBuffers.begin() + static_cast<int>(toBeSwapped), m_PixelBuffers.begin() + static_cast<int>(swappedWith));

}
