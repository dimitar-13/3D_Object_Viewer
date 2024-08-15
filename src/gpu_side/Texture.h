#pragma once
#include "pch.h"
#include "Core/CommonAppData.h"
#include "Enums/TextureEnums.h"
#include "Enums/GpuDataTypes.h"
namespace OBJ_Viewer {
#pragma region TextureStructs
    /**
     * @brief Reserved for when we switch from builder pattern to raw structs. 
     */
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
        /**
         * @brief Constructs a Texture object with specified parameters.
         *
         * Constructor copies all of the parameter,creates a texture object and determines the target based on if we have multi-sample or not
         * and then it calls the setup function 'SetTextureProperties()'.
         *
         * @param texture_pixel_data Pointer to the raw texture data. The data should be in the format specified by `texture_format`.
         * @param texture_internal_format Specifies the internal format of the texture, describing how should OpenGL interprets our texture data of the time is the same as 'm_textureFormat'
         * @param texture_format Specifies the format of the texture data (e.g., GL_RGBA, GL_RGB).
         * @param texture_size Size of the texture in pixels (width and height).
         * @param texture_data_type Specifies the data type of the pixel data (e.g., GL_UNSIGNED_BYTE).
         * @param texture_wrapping_mode_on_S Texture wrapping mode for the S (U) coordinate (e.g., GL_REPEAT, GL_CLAMP_TO_EDGE).
         * @param texture_wrapping_mode_on_T Texture wrapping mode for the T (V) coordinate (e.g., GL_REPEAT, GL_CLAMP_TO_EDGE).
         * @param is_texture_multi_sampled Boolean flag indicating whether the texture should be multi-sampled.
         * @param sample_count Number of samples per pixel for multi-sampled textures. Only relevant if `is_texture_multi_sampled` is true.
         */
		Texture(const unsigned char* texture_pixel_data, TextureInternalFormat_ texture_internal_format,
			TextureFormat_ texture_format, Size2D texture_size, GPUDataType_ texture_data_type, TextureWrap_ texture_wrapping_mode_on_S,
			TextureWrap_ texture_wrapping_mode_on_T, bool is_texture_multi_sampled, uint8_t sample_count);
        /**
         * @brief Release the GPU-based texture resource. 
         */
        ~Texture(){ glDeleteTextures(1, &this->m_textureHandle); }
        /**
         * @brief Binds the texture to the current OpenGL context.
         *
         * This function binds the texture represented by this object to the specified target in the OpenGL context.
         * It makes the texture active so that it can be used for rendering operations.
         */
		void BindTexture()const {glBindTexture(m_target, this->m_textureHandle);}
        /**
         * @brief Unbinds the texture from the current OpenGL context.
         *
         * This function unbinds the texture by setting the current texture binding to 0 for the specified target.
         * It effectively deactivates the texture, ensuring no texture is bound to the target.
         */
		void UnbindTexture()const {glBindTexture(m_target, 0);}
        /**
         * @brief Retrieves the handle of the texture.
         *
         * This function returns the OpenGL handle (ID) of the texture. The handle is used to reference the texture
         * in OpenGL operations. This is mainly used for the ImGUI texture rendering.
         *
         * @return The OpenGL handle (ID) of the texture.
         */
		GLuint GetTextureHandle()const { return m_textureHandle; }
        /**
         * @brief Retrieves the format of the texture.
         *
         * This function returns the format of the texture, which describes the internal layout of the texture data.
         *
         * @return The format of the texture as a TextureFormat_ enum.
         */
		TextureFormat_ GetTextureFormat()const { return m_textureFormat; }
        /**
         * @brief Resizes the texture to the specified dimensions.
         *
         * This function resizes the texture to the new dimensions provided by the `new_texture_size` parameter.
         * The texture data is reallocated according to the new size, and the texture settings are updated accordingly.
         * If the `new_texture_size` is the same as the current one the function simply returns.
         *
         * @param new_texture_size The new dimensions for the texture.
         */
		void ResizeTexture(Size2D new_texture_size);
	private:
        /**
         * @brief Initialize the 2D texture information. 
         *
         * This function initializes a texture with the given pixel data and configuration settings.
         * It allows specifying the internal format, format, size, data type, and wrapping modes for the texture.
         * 
         * @param texture_pixel_data Pointer to the raw texture data. The data should be in the format specified by `texture_format`.
         */
		void SetTextureProperties(const unsigned char* texture_pixel_data);
	private:
		Size2D m_textureSize;                                     ///< Current texture size.
		TextureWrap_ m_textureWrapS;                              ///< Texture wrapping mode for the S (U) coordinate. Determines how texture coordinates outside the range [0, 1] are handled in the horizontal direction.
		TextureWrap_ m_textureWrapT;                              ///< Texture wrapping mode for the T (V) coordinate. Determines how texture coordinates outside the range [0, 1] are handled in the vertical direction. 
		TextureFormat_ m_textureFormat;                           ///< Format of the texture, describing how the texture data is stored (e.g., RGB, RGBA). 
		TextureInternalFormat_ m_textureInternalFormat;           ///< Internal format of the texture, describing how should OpenGL interprets our texture 99.99% of the time is the same as 'm_textureFormat'. 
		GPUDataType_ m_texturePixelDataType;                      ///< Data type of the pixel data (e.g., GL_UNSIGNED_BYTE). Determines how the data is read from memory. 
		GLenum m_target;                                          ///< Target to which the texture is bound within the texture class will be the GL_TEXTURE_2D or GL_TEXTURE_2D_MULTISAMPLE. 
		bool m_isMultiSample;                                     ///< Flag indicating if the texture is a multi-sample texture (true if multi-sampled, false otherwise). 
		uint8_t m_sampleCount;                                    ///< Number of samples per pixel in the texture. Used only if m_isMultiSample is true. 
		GLuint m_textureHandle;                                   ///< OpenGL handle/ID representing the texture. Use this for texture related API calls.
	};


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

