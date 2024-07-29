#pragma once
#include "pch.h"
#include "Core/CommonAppData.h"
#include "TextureEnums.h"
namespace OBJ_Viewer {
#pragma region TextureStructs	
	struct TextureData
	{
		Size2D TextureSize;
		TextureFormat m_textureFormat;
		TextureInternalFormat m_textureInternalFormat;
	};
#pragma endregion

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
		TextureFormat GetTextureFormat()const { return m_textureFormat; }
		void ResizeTexture(Size2D newSize);
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

