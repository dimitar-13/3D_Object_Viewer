#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<iostream>
OBJ_Viewer::Texture::Texture(const unsigned char* data, TextureInternalFormat textureInternalFormat, TextureFormat textureFormat,
	TextureSize textureSize, TexturePixelDataType dataType, TextureWrap textureWrapS, TextureWrap textureWrapT)
	:m_textureSize(textureSize),m_textureWrapS(textureWrapS), m_textureWrapT(textureWrapT),
	m_textureFormat(textureFormat), m_textureInternalFormat(textureInternalFormat),
	m_texturePixelDataType(dataType)
{
	glGenTextures(1, &this->m_textureHandle);
	SetTextureProperties(data);
}

OBJ_Viewer::Texture::~Texture()
{
	glDeleteTextures(1, &this->m_textureHandle);
}

void OBJ_Viewer::Texture::BindTexture() const
{
	glBindTexture(1, this->m_textureHandle);
}

void OBJ_Viewer::Texture::ResizeTexture(TextureSize newSize)
{
	this->m_textureSize = newSize;
	SetTextureProperties(nullptr);
}

std::shared_ptr<unsigned char[]> OBJ_Viewer::Texture::GetPixelData()
{
	//Should be based on the type of pixel data;
	const int ArrSize = this->m_textureSize.height * this->m_textureSize.height * sizeof(unsigned char);
	std::shared_ptr<unsigned char[]> my_array(new unsigned char[ArrSize]);
	glGetTexImage(GL_TEXTURE_2D, 0, this->m_textureFormat, this->m_texturePixelDataType, my_array.get());
	return my_array;
}

void OBJ_Viewer::Texture::SetTextureProperties(const unsigned char* data)
{
	glBindTexture(GL_TEXTURE_2D, this->m_textureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, m_textureInternalFormat,
		m_textureSize.width, m_textureSize.height, 0, m_textureFormat,m_texturePixelDataType,data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_textureWrapT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OBJ_Viewer::TexturePixelDataWrapper::TexturePixelDataWrapper(const char* path, TextureSize* pTextureSize, int* pPresentChannelsCount)
{
	stbi_set_flip_vertically_on_load(true);
	m_pixelData = stbi_load(path, &pTextureSize->width, &pTextureSize->height, pPresentChannelsCount, 0);
	if (!m_pixelData)
	{	
		std::cout << "[ERROR]:STBI_IMAGE failed to read or allocate texture at path." << path << '\n';
	}
}

OBJ_Viewer::TexturePixelDataWrapper::~TexturePixelDataWrapper()
{
	if (m_pixelData)
		stbi_image_free(m_pixelData);
}
