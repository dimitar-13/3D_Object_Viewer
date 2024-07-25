#pragma once
#include "pch.h"
#include "gpu_side/TextureEnums.h"
#include "Core/CommonAppData.h"
#include "Logging/App_Logger.h"
namespace OBJ_Viewer {

	inline TextureFormat GetFormatByChannelCount(int channelCount)
	{
		/* If the channel count is higher than 2 it means we have either RGB or RGBA format
		* so far we will calculate those: because they are sequential.We can get the channel count
		* minus the lowest of the 2 counts witch is 3 and added to the smaller 'TEXTURE_FORMAT_RGB'
		* example:
		* We receive 3 in this case 3 - 3 is 0 meaning we have RGB in other case we have 1 witch is RBGA
		* If we have lower than 2 than we either have single R channel or a RG channel easiest way here
		* to see if we have division by 2 (without decimal numbers).
		* Have we made the enums increment in order we can avoid this ? Yes but openGL macros are not in order
		* so either enum in order and fetch OPENGL macros or this.
		* Some might see that there is no big change here but for debugging and for future changes its better since atleast we can change
		* the render API if we want and the program will still work (maybe hypocritical considering in some places we use GL typedef data).
		*/
		return channelCount > 2 ? static_cast<TextureFormat>((channelCount - 3) + TEXTURE_FORMAT_RGB)
			: channelCount % 2 == 0.0f ? TEXTURE_FORMAT_R : TEXTURE_FORMAT_RG;
	}

	struct TexturePixelReader
	{
	public:
		TexturePixelReader(const char* path)
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
			m_channelCount = static_cast<uint8_t>(channelCount);
			m_textureFormat = GetFormatByChannelCount(channelCount);
		}
		~TexturePixelReader(){ stbi_image_free(m_pixelData); }
		unsigned char* GetTexturePixelData()const { return m_pixelData; }
		Size2D GetTextureSize()const { return m_textureSize; }
		TextureFormat GetTextureFormat()const { return m_textureFormat; }
		bool isTextureValid()const { return m_pixelData != nullptr; }
		uint8_t GetChannelCount()const { return m_channelCount; }
	private:
		unsigned char* m_pixelData = nullptr;
		Size2D m_textureSize {};
		TextureFormat m_textureFormat{};
		uint8_t m_channelCount {};
	};
	
}