#include "pch.h"
#include "TextureHelpers.h"

#pragma region Texture format converter src
constexpr OBJ_Viewer::TextureFormat OBJ_Viewer::TextureFormatEnumConverter::GetFormatByChannelCount(int channelCount)
{
	switch (channelCount)
	{
	case 1:
		return TEXTURE_FORMAT_R;
		break;
	case 2:
		return TEXTURE_FORMAT_RG;
		break;
	case 3:
		return TEXTURE_FORMAT_RGB;
		break;
	case 4:
		return TEXTURE_FORMAT_RGBA;
		break;
	default:
		break;
	}
}

constexpr uint8_t OBJ_Viewer::TextureFormatEnumConverter::GetChannelCountByFormat(TextureFormat format)
{
	switch (format)
	{
	case OBJ_Viewer::TEXTURE_FORMAT_UKNOWN:
		return 0;
		break;
	case OBJ_Viewer::TEXTURE_FORMAT_R:
		return 1;
		break;
	case OBJ_Viewer::TEXTURE_FORMAT_RG:
		return 2;
		break;
	case OBJ_Viewer::TEXTURE_FORMAT_RGB:
		return 3;
		break;
	case OBJ_Viewer::TEXTURE_FORMAT_RGBA:
		return 4;
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region Image saver src

int OBJ_Viewer::TexturePixelSaver::SavePicture(const std::string& filePath_name, Size2D imageSize,TextureFormat textureChanelFormat,
	std::shared_ptr<std::vector<unsigned char>> pixelDataToSave, ImageFileFormat imageSaveFormat)
{
	switch (imageSaveFormat)
	{
	case ImageFileFormat::IMAGE_FORMAT_PNG:
		return SavePNG(filePath_name.c_str(), imageSize, textureChanelFormat,(void*)(pixelDataToSave->data()));
		break;
	case ImageFileFormat::IMAGE_FORMAT_JPEG:
		return SaveJPEG(filePath_name.c_str(), imageSize, textureChanelFormat, (void*)pixelDataToSave->data());
		break;
	case ImageFileFormat::IMAGE_FORMAT_BMP:
			return SaveBitmap(filePath_name.c_str(), imageSize, textureChanelFormat,(void*)pixelDataToSave->data());
			break;
	default:
		break;
	}
}

int OBJ_Viewer::TexturePixelSaver::SaveJPEG(const char* filePath_name, Size2D imageSize, TextureFormat saveFormat, void* pixelDataToSave)
{
	constexpr int JPEG_QUALITY = 50;
	int result = stbi_write_jpg(filePath_name, imageSize.width, imageSize.height,
		TextureFormatEnumConverter::GetChannelCountByFormat(saveFormat),
		pixelDataToSave, JPEG_QUALITY);
	if (result == 0)
	{
		LOGGER_LOG_ERROR("STBI_IMAGE failed to save the image to path:'{0}'", filePath_name);
	}
	return result;
}

int OBJ_Viewer::TexturePixelSaver::SavePNG(const char* filePath_name, Size2D imageSize, TextureFormat saveFormat, void* pixelDataToSave)
{
	const size_t imageStride = imageSize.width * TextureFormatEnumConverter::GetChannelCountByFormat(saveFormat);

	int result = stbi_write_png(filePath_name, imageSize.width, imageSize.height,
		TextureFormatEnumConverter::GetChannelCountByFormat(saveFormat),pixelDataToSave, imageStride);

	if (result == 0)
	{
		LOGGER_LOG_ERROR("STBI_IMAGE failed to save the image to path:'{0}'", filePath_name);
	}
	return result;
}
int OBJ_Viewer::TexturePixelSaver::SaveBitmap(const char* filePath_name, Size2D imageSize, TextureFormat saveFormat, void* pixelDataToSave)
{
	const size_t imageStride = imageSize.width * TextureFormatEnumConverter::GetChannelCountByFormat(saveFormat);

	int result = stbi_write_bmp(filePath_name, imageSize.width, imageSize.height,
		TextureFormatEnumConverter::GetChannelCountByFormat(saveFormat), pixelDataToSave);

	if (result == 0)
	{
		LOGGER_LOG_ERROR("STBI_IMAGE failed to save the image to path:'{0}'", filePath_name);
	}
	return result;
}
#pragma endregion

#pragma region Pixel reader src

OBJ_Viewer::TexturePixelReader::TexturePixelReader(const char* path)
{
	int channelCount;

	m_pixelData = stbi_load(path, &m_textureSize.width, &m_textureSize.height, &channelCount, 0);
	if (!m_pixelData)
	{
		LOGGER_LOG_ERROR("STBI_IMAGE failed to read or allocate texture at path:'{0}'", path);
		m_textureFormat = TEXTURE_FORMAT_UKNOWN;
		m_textureSize = { 0,0 };
		return;
	}
	m_textureFormat = TextureFormatEnumConverter::GetFormatByChannelCount(channelCount);
}
#pragma endregion

std::string_view OBJ_Viewer::TextureFileEnumConverter::GetStringTextureFormatFromEnum(ImageFileFormat val)
{
	//A little bit "hacky" but it avoids some code duplication

	return GetStringTextureFileExtensionFormatFromEnum(val).substr(1);
}

std::string_view OBJ_Viewer::TextureFileEnumConverter::GetStringTextureFileExtensionFormatFromEnum(ImageFileFormat val)
{
	switch (val)
	{
	case ImageFileFormat::IMAGE_FORMAT_PNG:
		return ".png";
		break;
	case ImageFileFormat::IMAGE_FORMAT_JPEG:
		return ".jpeg";
		break;
	case ImageFileFormat::IMAGE_FORMAT_BMP:
		return ".bmp";
		break;
	default:
		return std::string_view();
		break;
	}
}
