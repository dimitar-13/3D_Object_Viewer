#include "pch.h"
#include "Texture.h"
#include"Logging/App_Logger.h"

OBJ_Viewer::Texture::Texture(const unsigned char* texture_pixel_data, TextureInternalFormat_ texture_internal_format,
	TextureFormat_ texture_format,Size2D texture_size, GPUDataType_ texture_data_type, TextureWrap_ texture_wrapping_mode_on_S,
	TextureWrap_ texture_wrapping_mode_on_T, bool is_texture_multi_sampled,uint8_t sample_count):

	m_textureSize(texture_size),m_textureWrapS(texture_wrapping_mode_on_S), m_textureWrapT(texture_wrapping_mode_on_T),
	m_textureFormat(texture_format), m_textureInternalFormat(texture_internal_format),
	m_texturePixelDataType(texture_data_type),m_isMultiSample(is_texture_multi_sampled),m_sampleCount(sample_count)
{
	m_target = is_texture_multi_sampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	glGenTextures(1, &this->m_textureHandle);
	SetTextureProperties(texture_pixel_data);
}

OBJ_Viewer::Texture::~Texture()
{
	glDeleteTextures(1, &this->m_textureHandle);
}

void OBJ_Viewer::Texture::ResizeTexture(Size2D new_texture_size)
{
	this->m_textureSize = new_texture_size;
	SetTextureProperties(nullptr);
}

void OBJ_Viewer::Texture::SetTextureProperties(const unsigned char* texture_pixel_data)
{
	glBindTexture(m_target, this->m_textureHandle);

	if (!m_isMultiSample)
	{
		glTexImage2D(m_target, 0, m_textureInternalFormat,
			m_textureSize.width, m_textureSize.height, 0, m_textureFormat, m_texturePixelDataType, texture_pixel_data);

		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_textureWrapS);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_textureWrapT);
	}
	else
	{
		glTexImage2DMultisample(m_target, m_sampleCount, m_textureInternalFormat,
			m_textureSize.width, m_textureSize.height, GL_TRUE);
	}
	glBindTexture(m_target, 0);
}
