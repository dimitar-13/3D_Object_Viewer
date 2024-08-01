#include "pch.h"
#include "Skybox.h"
#include "Helpers/MeshGeneratingMethods.h"

OBJ_Viewer::Skybox::Skybox(const std::array<TexturePixelReader, Skybox::SKYBOX_FACE_COUNT>& textures):
	m_skyboxVAO(GenerateCubeVAO(true))
{

	const Size2D skybox_texture_size = textures[0].GetTextureSize();

	const size_t kSkyboxPixelBufferByteSize = static_cast<size_t>(skybox_texture_size.height)
		* static_cast<size_t>(skybox_texture_size.width) * textures[0].GetChannelCount();

	BufferData pixel_buffer_info;
	pixel_buffer_info.type = BufferType_kPixelUnpackBuffer;
	pixel_buffer_info.usageType = BufferUsageType_kStreamDraw;
	pixel_buffer_info.bufferSize = kSkyboxPixelBufferByteSize;

	m_CubeMapTextSize = skybox_texture_size;
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
	

	for (size_t i = 0; i < SKYBOX_FACE_COUNT; i++)
	{	
		pixel_buffer_info.data = textures[i].GetTexturePixelData();

		m_PixelBuffers[i] = std::make_unique<OpenGLBuffer>(pixel_buffer_info);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_format, skybox_texture_size.width, skybox_texture_size.height,
			0, m_format, GL_UNSIGNED_BYTE, textures[i].GetTexturePixelData());

		TextureBuilder builder;

		m_faceTextures[i] = builder.SetTextureFormat(m_format).
			SetTextureInternalFormat(static_cast<TextureInternalFormat_>(m_format))
			.SetTextureSize(skybox_texture_size).SetTexturePixelData(textures[i].GetTexturePixelData()).buildTexture();
	}

	m_faceTextures[0]->UnbindTexture();
	m_PixelBuffers[0]->UnbindBuffer();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OBJ_Viewer::Skybox::~Skybox()
{
	glDeleteTextures(1, &this->m_cubeMapHandle);
}

void OBJ_Viewer::Skybox::SwapSkyboxFaceTextures(SkyboxFace_ toBeSwapped, SkyboxFace_ swappedWith)
{
	if (toBeSwapped == swappedWith)
		return;

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle);

	m_PixelBuffers[swappedWith]->BindBuffer();
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + toBeSwapped, 0, 0, 0, m_CubeMapTextSize.width, m_CubeMapTextSize.height, m_format, GL_UNSIGNED_BYTE, nullptr);
    m_PixelBuffers[swappedWith]->UnbindBuffer();


	m_PixelBuffers[toBeSwapped]->BindBuffer();
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + swappedWith, 0, 0, 0, m_CubeMapTextSize.width ,m_CubeMapTextSize.height, m_format, GL_UNSIGNED_BYTE, nullptr);
    m_PixelBuffers[toBeSwapped]->UnbindBuffer();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	std::iter_swap(m_faceTextures.begin() + static_cast<int>(toBeSwapped), m_faceTextures.begin() + static_cast<int>(swappedWith));
	std::iter_swap(m_PixelBuffers.begin() + static_cast<int>(toBeSwapped), m_PixelBuffers.begin() + static_cast<int>(swappedWith));

}
