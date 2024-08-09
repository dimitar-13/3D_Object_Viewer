#pragma once
#include "pch.h"
#include "Core/CommonAppData.h"
#include "Enums/TextureEnums.h"
#include "Enums/GpuDataTypes.h"
namespace OBJ_Viewer {
#pragma region TextureStructs	
	struct TextureData
	{
		Size2D TextureSize;
		TextureFormat_ m_textureFormat;
		TextureInternalFormat_ m_textureInternalFormat;
	};
#pragma endregion

	class Texture
	{
	public:
		Texture(const unsigned char* texture_pixel_data, TextureInternalFormat_ texture_internal_format,
			TextureFormat_ texture_format, Size2D texture_size, GPUDataType_ texture_data_type, TextureWrap_ texture_wrapping_mode_on_S,
			TextureWrap_ texture_wrapping_mode_on_T, bool is_texture_multi_sampled, uint8_t sample_count);
		~Texture();
		void BindTexture()const {glBindTexture(m_target, this->m_textureHandle);}
		void UnbindTexture()const {glBindTexture(m_target, 0);}
		GLuint GetTextureHandle()const { return m_textureHandle; }
		TextureFormat_ GetTextureFormat()const { return m_textureFormat; }
		void ResizeTexture(Size2D new_texture_size);
	private:
		void SetTextureProperties(const unsigned char* texture_pixel_data);
	private:
		Size2D m_textureSize;
		TextureWrap_ m_textureWrapS;
		TextureWrap_ m_textureWrapT;
		TextureFormat_ m_textureFormat;
		TextureInternalFormat_ m_textureInternalFormat;
		GPUDataType_ m_texturePixelDataType;
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
		TextureBuilder& SetTextureFormat(TextureFormat_ format) { this->textureFormat = format; return *this; }
		TextureBuilder& SetTextureInternalFormat(TextureInternalFormat_ internalFormat) { this->textureInternalFormat = internalFormat; return *this; }
		TextureBuilder& SetTextureWrapS(TextureWrap_ textureWrap) { this->textureWrapS = textureWrap; return *this; }
		TextureBuilder& SetTextureWrapT(TextureWrap_ textureWrap) { this->textureWrapT = textureWrap; return *this; }
		TextureBuilder& SetTextureSize(Size2D size) { this->textureSize = size; return *this; }
		TextureBuilder& SetTexturePixelData(const unsigned char* data) { this->data = data; return *this; }
		TextureBuilder& SetTexturePixelDataType(GPUDataType_ dataType) { this->texturePixelDataType = dataType; return *this; }
		TextureBuilder& isTextureMultiSample(bool val) { isMultiSample = val;  return *this;}
		TextureBuilder& SetSampleCount(uint8_t sampleCount) { texSampleCount = sampleCount;  return *this;}

		std::unique_ptr<Texture> buildTexture()const
		{ return std::make_unique<Texture>(data, textureInternalFormat, textureFormat, textureSize,
			texturePixelDataType, textureWrapS, textureWrapT, isMultiSample, texSampleCount); }
	private:
		TextureWrap_ textureWrapS = TextureWrap_::TextureWrap_kRepeat;
		TextureWrap_ textureWrapT = TextureWrap_::TextureWrap_kRepeat;
		Size2D textureSize = {500,500};
		TextureFormat_ textureFormat;
		TextureInternalFormat_ textureInternalFormat;
		GPUDataType_ texturePixelDataType = GPUDataType_::GPUDataType_kUnsignedByte;
		const unsigned char* data = nullptr;
		bool isMultiSample = false;
		uint8_t texSampleCount = 4;
	};
}

