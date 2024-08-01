#pragma once
#include "pch.h"
#include "gpu_side/TextureEnums.h"
#include "Core/CommonAppData.h"
#include "Logging/App_Logger.h"
#include "Enums/FIleImageEnums.h"

namespace OBJ_Viewer {

	//Not a good aproach switch case in this case will be more efficiant.
	// Letf this here as a showcase of misktaes and how to not let ur ego write functions like this 
	
	//inline TextureFormat GetFormatByChannelCount(int channelCount)
	//{
	//	/* If the channel count is higher than 2 it means we have either RGB or RGBA format
	//	* so far we will calculate those: because they are sequential.We can get the channel count
	//	* minus the lowest of the 2 counts witch is 3 and added to the smaller 'TEXTURE_FORMAT_RGB'
	//	* example:
	//	* We receive 3 in this case 3 - 3 is 0 meaning we have RGB in other case we have 1 witch is RBGA
	//	* If we have lower than 2 than we either have single R channel or a RG channel easiest way here
	//	* to see if we have division by 2 (without decimal numbers).
	//	* Have we made the enums increment in order we can avoid this ? Yes but openGL macros are not in order
	//	* so either enum in order and fetch OPENGL macros or this.
	//	* Some might see that there is no big change here but for debugging and for future changes its better since atleast we can change
	//	* the render API if we want and the program will still work (maybe hypocritical considering in some places we use GL typedef data).
	//	*/
	//	return channelCount > 2 ? static_cast<TextureFormat>((channelCount - 3) + TEXTURE_FORMAT_RGB)
	//		: channelCount % 2 == 0.0f ? TEXTURE_FORMAT_R : TEXTURE_FORMAT_RG;
	//}

	struct TextureFormatEnumConverter
	{
		static constexpr TextureFormat_ GetFormatByChannelCount(int channelCount);
		static constexpr size_t GetChannelCountByFormat(TextureFormat_ format);
	};
	struct TextureFileEnumConverter
	{
		static std::string_view GetStringTextureFormatFromEnum(ImageFileFormat_ val);
		static std::string_view GetStringTextureFileExtensionFormatFromEnum(ImageFileFormat_ val);
	};


	class TexturePixelSaver
	{	
	public:
		static int SavePicture(std::string filePath_name, Size2D imageSize,
			TextureFormat_ textureChanelFormat, std::shared_ptr<std::vector<unsigned char>> pixelDataToSave,ImageFileFormat_ imageSaveFormat);
	private:
		static int SaveJPEG(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave);
		static int SavePNG(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave);
		static int SaveBitmap(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave);
	};

	class TexturePixelReader
	{
	public:
		TexturePixelReader(const char* path);
		~TexturePixelReader(){ stbi_image_free(m_pixelData); }
		unsigned char* GetTexturePixelData()const { return m_pixelData; }
        size_t GetTextureByteSize()const { return static_cast<size_t>(m_textureSize.width) * static_cast<size_t>(m_textureSize.height) * TextureFormatEnumConverter::GetChannelCountByFormat(m_textureFormat); }
		Size2D GetTextureSize()const { return m_textureSize; }
		TextureFormat_ GetTextureFormat()const { return m_textureFormat; }
		bool isTextureValid()const { return m_pixelData != nullptr; }
		uint8_t GetChannelCount()const { return TextureFormatEnumConverter::GetChannelCountByFormat(m_textureFormat); }
	private:
		unsigned char* m_pixelData = nullptr;
		Size2D m_textureSize {};
		TextureFormat_ m_textureFormat{};
	};
	
}