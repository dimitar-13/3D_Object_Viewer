#pragma once
#include "pch.h"
#include "Enums/TextureEnums.h"
#include "Core/CommonAppData.h"
#include "Logging/App_Logger.h"
#include "Enums/FIleImageEnums.h"

namespace OBJ_Viewer {

	// Not a good approach switch case in this case will be more efficient.
	// Left this here as a showcase of mistakes and how to not let ur ego write functions like this 
	
	/**
     * inline TextureFormat GetFormatByChannelCount(int channelCount)
	 * {
	 * 	// If the channel count is higher than 2 it means we have either RGB or RGBA format
	 * 	// so far we will calculate those: because they are sequential.We can get the channel count
	 * 	// minus the lowest of the 2 counts witch is 3 and added to the smaller 'TEXTURE_FORMAT_RGB'
	 * 	// example:
	 * 	// We receive 3 in this case 3 - 3 is 0 meaning we have RGB in other case we have 1 witch is RBGA
	 * 	// If we have lower than 2 than we either have single R channel or a RG channel easiest way here
	 * 	// to see if we have division by 2 (without decimal numbers).
	 * 	// Have we made the enums increment in order we can avoid this ? Yes but openGL macros are not in order
	 * 	// so either enum in order and fetch OPENGL macros or this.
	 * 	// Some might see that there is no big change here but for debugging and for future changes its better since atleast we can change
	 * 	// the render API if we want and the program will still work (maybe hypocritical considering in some places we use GL typedef data).
	 * 	
	 * 	return channelCount > 2 ? static_cast<TextureFormat>((channelCount - 3) + TEXTURE_FORMAT_RGB)
	 * 		: channelCount % 2 == 0.0f ? TEXTURE_FORMAT_R : TEXTURE_FORMAT_RG;
	 }
     */


    /**
     * @brief Helper static class for converting TextureFormat_ enum to int values
     * 
     * This class have 2 utility functions that can be used to get the corresponding 
     * TextureFormat_ channel count or vise versa.
     */
	struct TextureFormatEnumConverter
	{
        /**
         * @brief Returns the corresponding enum based on the provided 'channelCount'
         * 
         * This function takes channel count as an input where channel count is the pixel channels like R,G,B(3) or R,G,B,A(4)
         * and returns the corresponding TextureFormat_ enum. If it cant find it or the channel count is not valid like -1 etc. 
         * It will return 'TextureFormat_kUnknown'.
         * 
         * @param channelCount Per pixel channels like R,G,B(3) or R,G,B,A(4). 
         * @returns The corresponding enum to channelCount
         */
		static constexpr TextureFormat_ GetFormatByChannelCount(int channelCount);
        /**
         * @brief Returns the channel count from the provided TextureFormat_ enum.
         * 
         * This function takes TextureFormat_ as the input and returns the corresponding pixel channel count(number).
         * See the TextureFormat_ enum for the current options.
         * 
         * @param format TextureFormat_ enum of the format you want to get the channel count.
         * 
         * @return Whole number(size_t) representing the challen count for R,G,B,A(4 components).
         */
		static constexpr size_t GetChannelCountByFormat(TextureFormat_ format);
	};
    /**
     *  Utility class for converting ImageFileFormat_ enum to string.
     * 
     * This is a static class containing functions for converting ImageFileFormat_ enum to string(string_view).
     */
	struct TextureFileEnumConverter
	{
		static std::string_view GetStringTextureFormatFromEnum(ImageFileFormat_ val);
		static std::string_view GetStringTextureFileExtensionFormatFromEnum(ImageFileFormat_ val);
	};

    /**
     * @brief Static class for saving texture to disc.
     *
     * This is static class that saves texture to a disc using 'stbi_image' https://github.com/nothings/stb library.
     * The class works formats defined in 'TextureFormat_' enum.The class have Save<format> functions that it will choose 
     * based on the format of the image you want to safe.
     * 
     * @note This class is designed to work async and its mainly used to save application screenshots.
     * Example use:
     * ```cpp
     * TexturePixelSaver::SavePicture(<data that this method takes>).
     * ```
     * For a better example see: Rendering/RenderingCoordinator.cpp/onEventTakeScreenshot function.
     */
	class TexturePixelSaver
	{	
	public:
        /**
         * @brief Function that saves an pixel data to a existing image file.
         *
         * This function takes an file name,size pixel format, file format and pixel data and saves it to disc.
         * The function is designed to be used async hence why we use std::shared_ptr<> for the vector and not a vector reference.
         * 
         * 
         * @param filePath_name The file path and name where the image will be saved.
         * @param imageSize The size of the image (width and height).
         * @param textureChanelFormat The format of the texture channels (e.g., RGB, RGBA).
         * @param pixelDataToSave Shared pointer to the pixel data vector to be saved.
         * @param imageSaveFormat The format in which the image will be saved (e.g., PNG, JPEG).
         * 
         * @returns True if the image was successfully saved, false otherwise.
         */
		static bool SavePicture(std::string filePath_name, Size2D imageSize,
			TextureFormat_ textureChanelFormat, std::shared_ptr<std::vector<unsigned char>> pixelDataToSave,ImageFileFormat_ imageSaveFormat);
	private:
        /**
         * @brief Saves an image to disk in JPEG format.
         *
         * This function saves the provided pixel data as a JPEG image file at the specified file path.
         *
         * @param filePath_name The file path where the JPEG image will be saved.
         * @param imageSize The size of the image (width and height).
         * @param saveFormat The format of the texture channels (e.g., RGB, RGBA).
         * @param pixelDataToSave Pointer to the pixel data to be saved.
         *
         * @returns True if the image was successfully saved, false otherwise.
         */
		static bool SaveJPEG(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave);
        /**
         * @brief Saves an image to disk in PNG format.
         *
         * This function saves the provided pixel data as a PNG image file at the specified file path.
         *
         * @param filePath_name The file path where the PNG image will be saved.
         * @param imageSize The size of the image (width and height).
         * @param saveFormat The format of the texture channels (e.g., RGB, RGBA).
         * @param pixelDataToSave Pointer to the pixel data to be saved.
         *
         * @returns True if the image was successfully saved, false otherwise.
         */
		static bool SavePNG(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave);
        /**
         * @brief Saves an image to disk in BMP format.
         *
         * This function saves the provided pixel data as a BMP (bitmap) image file at the specified file path.
         *
         * @param filePath_name The file path where the BMP image will be saved.
         * @param imageSize The size of the image (width and height).
         * @param saveFormat The format of the texture channels (e.g., RGB, RGBA).
         * @param pixelDataToSave Pointer to the pixel data to be saved.
         *
         * @returns True if the image was successfully saved, false otherwise.
         */
		static bool SaveBitmap(const char* filePath_name, Size2D imageSize, TextureFormat_ saveFormat, void* pixelDataToSave);
	};

    /**
     * @brief Wrapper class for reading textures.
     *
     * This class gets a texture path and using the library 'stbi_image'https://github.com/nothings/stb it allocates the texture and provides useful
     * information about the texture such as:
     * - Texture pixel channel count.
     * - Texture size.
     * - The texture data itself.
     * 
     * Example use:
     * ```cpp
     * TexturePixelReader reader(<absolute path to some texture>);
     * 
     * if(reader.isTextureValid())
     * {
     *    //Handle not valid texture;
     * }
     * else
     * {
     *    //You can work with ur texture.
     *    reader.GetTexturePixelData();
     *    reader.GetTextureSize();
     * }
     * 
     * ```
     */
	class TexturePixelReader
	{
	public:
        /**
         * @brief Constructor takes a absolute path and reads the texture. 
         * 
         * The constructor takes an absolute texture path. It reads the texture and extracts all the relevant information like
         * texture size,pixel channel count and pixel data. If the reads fail the pixel data is nullptr. You can use the 
         * 'isTextureValid()' to see if the read failed. To get the texture information you can use the following functions
         * 'GetTexturePixelData()','GetTextureByteSize()','GetTextureSize()' and etc. 'GetTextureXXX'.
         * 
         * @param texture_absolute_path The absolute path to the texture file.
         */
		TexturePixelReader(const char* texture_absolute_path);
        /**
         * @brief Frees the allocated texture resource. 
         */
		~TexturePixelReader(){ stbi_image_free(m_pixelData); }
        /**
         * @brief Returns the read texture pixel data array.
         *
         * Returns the read pixel data array.If texture read fails this will returns a nullptr. You can use the 
         * 'isTextureValid()' to check if the data is valid.
         * 
         * @returns The texture pixel data array.
         */
		unsigned char* GetTexturePixelData()const { return m_pixelData; }
        /**
         * @brief Returns the texture data array byte size.
         *
         * This function returns the read texture array size in bytes. Where the byte size is calculated as:
         * 'texture.width * texture.height * texture.pixel_channel_count'. If texture is not valid this method returns 0.
         * 
         * @returns The texture data array byte size.
         */
        size_t GetTextureByteSize()const { return static_cast<size_t>(m_textureSize.width) * static_cast<size_t>(m_textureSize.height) * TextureFormatEnumConverter::GetChannelCountByFormat(m_textureFormat); }
		/**
         * @brief Returns the size of the texture.
         * 
         * This function  returns the size of the read texture. If the texture is not valid this function returns Size2D{0,0}.
         * 
         * @returns The size of the texture.
         */
        Size2D GetTextureSize()const { return m_textureSize; }
        /**
         * @brief Returns the TextureFormat_ enum of the read texture.
         *
         * Returns the TextureFormat_ enum of the read texture. If the texture is not valid returns
         * TextureFormat_kUnknown.
         * 
         * @returns The TextureFormat_ enum of the read texture.
         */
		TextureFormat_ GetTextureFormat()const { return m_textureFormat; }
        /**
         * @brief Returns true if the texture is valid.
         * 
         * If the texture read was not successful or the texture is not valid this will return false.
         * @returns Returns true if the texture is valid and false if not.
         */
		bool isTextureValid()const { return m_pixelData != nullptr; }
        /**
         * @brief Returns the pixel channel count of the read texture.
         * 
         * Returns the pixel channel count of the read texture.
         * If the read texture is invalid this function will return 0.
         * 
         * @returns Pixel channel count of the read texture.
         */
		size_t GetTextureChannelCount()const { return TextureFormatEnumConverter::GetChannelCountByFormat(m_textureFormat); }
	private:
        unsigned char* m_pixelData = nullptr;   ///< The read pixel data.
		Size2D m_textureSize {};                ///< The read texture size.
		TextureFormat_ m_textureFormat{};       ///< The texture pixel format(RGB,RGBA etc.).
	};
	
}