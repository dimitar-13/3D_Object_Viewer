#include "pch.h"
#include "TextureHelpers.h"

#pragma region Texture format converter src
constexpr OBJ_Viewer::TextureFormat_ OBJ_Viewer::TextureFormatEnumConverter::GetFormatByChannelCount(int channelCount)
{
	switch (channelCount)
	{
	case 1:
		return TextureFormat_kR;
		break;
	case 2:
		return TextureFormat_kRG;
		break;
	case 3:
		return TextureFormat_kRGB;
		break;
	case 4:
		return TextureFormat_kRGBA;
		break;
	default:
		break;
	}
}

constexpr size_t OBJ_Viewer::TextureFormatEnumConverter::GetChannelCountByFormat(TextureFormat_ format)
{
	switch (format)
	{
	case OBJ_Viewer::TextureFormat_kUnknown:
		return 0;
		break;
	case OBJ_Viewer::TextureFormat_kR:
		return 1;
		break;
	case OBJ_Viewer::TextureFormat_kRG:
		return 2;
		break;
	case OBJ_Viewer::TextureFormat_kRGB:
		return 3;
		break;
	case OBJ_Viewer::TextureFormat_kRGBA:
		return 4;
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region Image saver src

int OBJ_Viewer::TexturePixelSaver::SavePicture(std::string filePath_name, Size2D imageSize,TextureFormat_ textureChanelFormat,
	std::shared_ptr<std::vector<unsigned char>> pixelDataToSave, ImageFileFormat_ imageSaveFormat)
{
	switch (imageSaveFormat)
	{
	case ImageFileFormat_::ImageFileFormat_kPNG:
		return SavePNG(filePath_name.c_str(), imageSize, textureChanelFormat,(void*)(pixelDataToSave->data()));
		break;
	case ImageFileFormat_::ImageFileFormat_kJPEG:
		return SaveJPEG(filePath_name.c_str(), imageSize, textureChanelFormat, (void*)pixelDataToSave->data());
		break;
	case ImageFileFormat_::ImageFileFormat_kBMP:
			return SaveBitmap(filePath_name.c_str(), imageSize, textureChanelFormat,(void*)pixelDataToSave->data());
			break;
	default:
		break;
	}
}

int OBJ_Viewer::TexturePixelSaver::SaveJPEG(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave)
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

int OBJ_Viewer::TexturePixelSaver::SavePNG(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave)
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
int OBJ_Viewer::TexturePixelSaver::SaveBitmap(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave)
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
		m_textureFormat = TextureFormat_kUnknown;
		m_textureSize = { 0,0 };
		return;
	}
	m_textureFormat = TextureFormatEnumConverter::GetFormatByChannelCount(channelCount);
}
#pragma endregion

std::string_view OBJ_Viewer::TextureFileEnumConverter::GetStringTextureFormatFromEnum(ImageFileFormat_ val)
{
	//A little bit "hacky" but it avoids some code duplication

	return GetStringTextureFileExtensionFormatFromEnum(val).substr(1);
}

std::string_view OBJ_Viewer::TextureFileEnumConverter::GetStringTextureFileExtensionFormatFromEnum(ImageFileFormat_ val)
{
	switch (val)
	{
	case ImageFileFormat_::ImageFileFormat_kPNG:
		return ".png";
		break;
	case ImageFileFormat_::ImageFileFormat_kJPEG:
		return ".jpeg";
		break;
	case ImageFileFormat_::ImageFileFormat_kBMP:
		return ".bmp";
		break;
	default:
		return std::string_view();
		break;
	}
}
