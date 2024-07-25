#include "pch.h"
#include "Texture.h"
#include"Logging/App_Logger.h"

OBJ_Viewer::Texture::Texture(const unsigned char* data, TextureInternalFormat textureInternalFormat, TextureFormat textureFormat,
	Size2D textureSize, TexturePixelDataType dataType, TextureWrap textureWrapS, TextureWrap textureWrapT, bool isMultiSample,
	uint8_t sampleCount)
	:m_textureSize(textureSize),m_textureWrapS(textureWrapS), m_textureWrapT(textureWrapT),
	m_textureFormat(textureFormat), m_textureInternalFormat(textureInternalFormat),
	m_texturePixelDataType(dataType),m_isMultiSample(isMultiSample),m_sampleCount(sampleCount)
{
	m_target = isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	glGenTextures(1, &this->m_textureHandle);
	SetTextureProperties(data);
}

OBJ_Viewer::Texture::~Texture()
{
	glDeleteTextures(1, &this->m_textureHandle);
}

void OBJ_Viewer::Texture::ResizeTexture(Size2D newSize)
{
	this->m_textureSize = newSize;
	SetTextureProperties(nullptr);
}

std::shared_ptr<unsigned char[]> OBJ_Viewer::Texture::GetPixelData()
{
	//Should be based on the type of pixel data;
	const int ArrSize = this->m_textureSize.height * this->m_textureSize.height * sizeof(unsigned char);
	std::shared_ptr<unsigned char[]> my_array(new unsigned char[ArrSize]);
	glGetTexImage(m_target, 0, this->m_textureFormat, this->m_texturePixelDataType, my_array.get());
	return my_array;
}

void OBJ_Viewer::Texture::SetTextureProperties(const unsigned char* data)
{
	glBindTexture(m_target, this->m_textureHandle);

	if (!m_isMultiSample)
	{
		glTexImage2D(m_target, 0, m_textureInternalFormat,
			m_textureSize.width, m_textureSize.height, 0, m_textureFormat, m_texturePixelDataType, data);

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
