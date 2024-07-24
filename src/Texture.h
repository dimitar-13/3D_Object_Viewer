#pragma once
#include "pch.h"
#include"CommonAppData.h"
namespace OBJ_Viewer {
#pragma region TextureStructs
	enum TextureFormat
	{
		TEXTURE_FORMAT_R = GL_RED,
		TEXTURE_FORMAT_RG = GL_RG,
		TEXTURE_FORMAT_RGB = GL_RGB,
		TEXTURE_FORMAT_RGBA = GL_RGBA
	};
	enum TextureInternalFormat {
		TEXTURE_INTERNAL_FORMAT_DEPTH = GL_DEPTH_COMPONENT,
		TEXTURE_INTERNAL_FORMAT_DEPTH_STENCIL = GL_DEPTH_STENCIL,
		TEXTURE_INTERNAL_FORMAT_R = GL_RED,
		TEXTURE_INTERNAL_FORMAT_RG = GL_RG,
		TEXTURE_INTERNAL_FORMAT_RGB = GL_RGB,
		TEXTURE_INTERNAL_FORMAT_RGBA = GL_RGBA,
	};
	enum TextureFilter
	{
		TEXTURE_FILTER_LINEAR = GL_LINEAR,
		TEXTURE_FILTER_NEAREST = GL_NEAREST
	};
	enum TextureWrap
	{
		TEXTURE_WRAP_REPEAT = GL_REPEAT,
		TEXTURE_WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		TEXTURE_WRAP_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,

		//TODO:Add more option
	};
	enum TexturePixelDataType
	{
		TEXTURE_PIXEL_DATA_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
		TEXTURE_PIXEL_DATA_BYTE = GL_BYTE,
		TEXTURE_PIXEL_DATA_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
		TEXTURE_PIXEL_DATA_SHORT = GL_SHORT,
		TEXTURE_PIXEL_DATA_UNSIGNED_INT = GL_UNSIGNED_INT,
		TEXTURE_PIXEL_DATA_INT = GL_INT,
		TEXTURE_PIXEL_DATA_HALF_FLOAT=GL_HALF_FLOAT,
		TEXTURE_PIXEL_DATA_FLOAT=GL_FLOAT
	};
	
	
	struct TextureData
	{
		Size2D TextureSize;
		TextureFormat m_textureFormat;
		TextureInternalFormat m_textureInternalFormat;
	};
#pragma endregion

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
			: channelCount % 2 == 0.0f ? TEXTURE_FORMAT_R: TEXTURE_FORMAT_RG;
	}

	struct TexturePixelDataWrapper
	{
	public:
		TexturePixelDataWrapper(const char* path, Size2D* pTextureSize,int* pPresentChannelsCount);
		~TexturePixelDataWrapper();
		unsigned char* GetTexturePixelData()const { return m_pixelData; }
	private:
		unsigned char* m_pixelData;
	};

	class Texture
	{
	public:
		Texture(const unsigned char* data, TextureInternalFormat textureInternalFormat, TextureFormat textureFormat,
			Size2D textureSize, TexturePixelDataType dataType, TextureWrap textureWrapS, TextureWrap textureWrapT,bool isMultiSample,
			uint8_t sampleCount);
		~Texture();
		void BindTexture()const {glBindTexture(m_target, this->m_textureHandle);}
		void UnbindTexture()const {glBindTexture(m_target, 0);}
		GLuint GetTextureHandle()const { return m_textureHandle; }
		void ResizeTexture(Size2D newSize);
		//Not tested;
		std::shared_ptr<unsigned char[]> GetPixelData();
	private:
		void SetTextureProperties(const unsigned char* data);
	private:
		Size2D m_textureSize;
		TextureWrap m_textureWrapS;
		TextureWrap m_textureWrapT;
		TextureFormat m_textureFormat;
		TextureInternalFormat m_textureInternalFormat;
		TexturePixelDataType m_texturePixelDataType;
		GLenum m_target;
		bool m_isMultiSample;
		uint8_t m_sampleCount;
		GLuint m_bindingPoint;
		GLuint m_textureHandle;
	};
	//(NOTE)Consider factory method maybe.Since if we add more texture types as cubeMap and so on altho this also might not the best idea since nothing really changes 
	//beside a filed and maybe 2 conditions not sure if its worth the code complication?
	class TextureBuilder
	{
	public:
		TextureBuilder& SetTextureFormat(TextureFormat format) { this->textureFormat = format; return *this; }
		TextureBuilder& SetTextureInternalFormat(TextureInternalFormat internalFormat) { this->textureInternalFormat = internalFormat; return *this; }
		TextureBuilder& SetTextureWrapS(TextureWrap textureWrap) { this->textureWrapS = textureWrap; return *this; }
		TextureBuilder& SetTextureWrapT(TextureWrap textureWrap) { this->textureWrapT = textureWrap; return *this; }
		TextureBuilder& SetTextureSize(Size2D size) { this->textureSize = size; return *this; }
		TextureBuilder& SetTexturePixelData(const unsigned char* data) { this->data = data; return *this; }
		TextureBuilder& SetTexturePixelDataType(TexturePixelDataType dataType) { this->texturePixelDataType = dataType; return *this; }
		TextureBuilder& isTextureMultiSample(bool val) { isMultiSample = val;  return *this;}
		TextureBuilder& SetSampleCount(uint8_t sampleCount) { texSampleCount = sampleCount;  return *this;}

		std::unique_ptr<Texture> buildTexture()const
		{ return std::make_unique<Texture>(data, textureInternalFormat, textureFormat, textureSize,
			texturePixelDataType, textureWrapS, textureWrapT, isMultiSample, texSampleCount); }
	private:
		TextureWrap textureWrapS = TextureWrap::TEXTURE_WRAP_REPEAT;
		TextureWrap textureWrapT = TextureWrap::TEXTURE_WRAP_REPEAT;
		Size2D textureSize = {500,500};
		TextureFormat textureFormat;
		TextureInternalFormat textureInternalFormat;
		TexturePixelDataType texturePixelDataType = TexturePixelDataType::TEXTURE_PIXEL_DATA_UNSIGNED_BYTE;
		const unsigned char* data = nullptr;
		bool isMultiSample = false;
		uint8_t texSampleCount = 4;
	};
}

