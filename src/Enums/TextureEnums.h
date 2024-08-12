#pragma once
#include "pch.h"
namespace OBJ_Viewer {

    /**
     * @brief Enum representing texture formats used in GPU operations.
     *
     * This enum defines various texture formats that can be used for specifying the format of texture data in GPU memory.
     * Each enum value corresponds to a specific OpenGL format, allowing for type-safe selection of texture formats.
     * This enum corresponds to the pixel data for example if you have a 3 component that that is a TextureFormat_kRGB.
     * If you want to specify how to interpret your pixel data and not the pixel component count see TextureInternalFormat_.
     * 
     *
     * - `TextureFormat_kUnknown`: Represents an unknown or unsupported texture format.
     * - `TextureFormat_kR`: Represents the `GL_RED` format, typically used for single-channel (red) textures.
     * - `TextureFormat_kRG`: Represents the `GL_RG` format, typically used for two-channel (red and green) textures.
     * - `TextureFormat_kRGB`: Represents the `GL_RGB` format, typically used for three-channel (red, green, and blue) textures.
     * - `TextureFormat_kRGBA`: Represents the `GL_RGBA` format, typically used for four-channel (red, green, blue, and alpha) textures.
     */
	enum TextureFormat_
	{
		TextureFormat_kUnknown, ///< For debugging.
		TextureFormat_kR = GL_RED, ///<Single-channel not specifically only Red but just a single pixel component.
		TextureFormat_kRG = GL_RG, ///<Two-channel format not specifically only Red and Green but just a single pixel component.
		TextureFormat_kRGB = GL_RGB, ///<Three-channel format not specifically only Red,Green and Blue but just a single pixel component..
		TextureFormat_kRGBA = GL_RGBA ///< Four-channel format, typically used for red, green, blue, and alpha, but can represent any four pixel components.
	};

    /**
      * @brief Enum representing internal formats for textures in GPU memory.
      *
      * This enum defines various internal formats that can be used when creating textures in GPU memory. Each format
      * corresponds to a specific OpenGL internal format, determining how the texture data is stored on the GPU.
      *
      * - 'TextureInternalFormat_kUnknown': For debugging. 
      * - `TextureInternalFormat_kDepth`: Represents a depth component format (`GL_DEPTH_COMPONENT`), used for depth textures.
      * - `TextureInternalFormat_kDepth_and_stencil`: Represents a combined depth and stencil format (`GL_DEPTH_STENCIL`), used when both depth and stencil data are needed.
      * - `TextureInternalFormat_kR`: Represents a single-channel format (`GL_RED`), typically used for storing only one component, such as red.
      * - `TextureInternalFormat_kRG`: Represents a two-channel format (`GL_RG`), typically used for storing two components, such as red and green.
      * - `TextureInternalFormat_kRGB`: Represents a three-channel format (`GL_RGB`), used for storing red, green, and blue components.
      * - `TextureInternalFormat_kRGBA`: Represents a four-channel format (`GL_RGBA`), used for storing red, green, blue, and alpha components.
      * - `TextureInternalFormat_kSRGB`: Represents a gamma-corrected RGB format (`GL_SRGB`), used for storing RGB data in the sRGB color space.
      */
    enum TextureInternalFormat_ {
        TextureInternalFormat_kUnknown, ///< For debugging. 
        TextureInternalFormat_kDepth = GL_DEPTH_COMPONENT, ///< Represents a depth component format (`GL_DEPTH_COMPONENT`), used for depth textures.
        TextureInternalFormat_kDepth_and_stencil = GL_DEPTH_STENCIL, ///< Represents a combined depth and stencil format (`GL_DEPTH_STENCIL`), used when both depth and stencil data are needed.
        TextureInternalFormat_kR = GL_RED, ///<Represents a single-channel format (`GL_RED`), typically used for storing only one component, such as red.
        TextureInternalFormat_kRG = GL_RG, ///< Represents a two-channel format (`GL_RG`), typically used for storing two components, such as red and green.
        TextureInternalFormat_kRGB = GL_RGB, ///< Represents a three-channel format (`GL_RGB`), used for storing red, green, and blue components.
        TextureInternalFormat_kRGBA = GL_RGBA, ///< Represents a four-channel format (`GL_RGBA`), used for storing red, green, blue, and alpha components.
        TextureInternalFormat_kSRGB = GL_SRGB ///< Represents a gamma-corrected RGB format (`GL_SRGB`), used for storing RGB data in the sRGB color space.
	};
    /**
     * @brief Enum representing texture filtering modes.
     *
     * This enum defines the filtering modes that can be applied to textures in OpenGL. Filtering determines how
     * textures are sampled, especially when they are magnified or minified.
     *
     * - `TextureFilter_kLinear`: Represents linear filtering (`GL_LINEAR`), where the GPU interpolates between multiple texels to produce a smooth texture.
     * - `TextureFilter_kNearest`: Represents nearest-neighbor filtering (`GL_NEAREST`), where the GPU selects the closest texel, resulting in a blocky texture.
     */
	enum TextureFilter_
	{
		TextureFilter_kLinear = GL_LINEAR,///< Represents linear filtering (`GL_LINEAR`), where the GPU interpolates between multiple texels to produce a smooth texture.
		TextureFilter_kNearest = GL_NEAREST ///< Represents nearest-neighbor filtering (`GL_NEAREST`), where the GPU selects the closest texel, resulting in a blocky texture.
	};
    /**
     * @brief Enum representing texture wrapping modes.
     *
     * This enum defines how texture coordinates outside the standard range [0, 1] are handled. Different wrapping
     * modes can be applied to control how textures repeat or clamp to the edges.
     *
     * - `TextureWrap_kRepeat`: Represents the repeat wrapping mode (`GL_REPEAT`), where the texture repeats indefinitely.
     * - `TextureWrap_kMirroredRepeat`: Represents the mirrored repeat wrapping mode (`GL_MIRRORED_REPEAT`), where the texture repeats but mirrors at every integer junction.
     * - `TextureWrap_kClampToEdge`: Represents the clamp-to-edge wrapping mode (`GL_CLAMP_TO_EDGE`), where texture coordinates outside [0, 1] are clamped to the edges of the texture.
     */
	enum TextureWrap_
	{
		TextureWrap_kRepeat = GL_REPEAT, ///< Represents the repeat wrapping mode (`GL_REPEAT`), where the texture repeats indefinitely.
		TextureWrap_kMirroredRepeat = GL_MIRRORED_REPEAT, ///< Represents the mirrored repeat wrapping mode (`GL_MIRRORED_REPEAT`), where the texture repeats but mirrors at every integer junction.
		TextureWrap_kClampToEdge = GL_CLAMP_TO_EDGE, ///< Represents the clamp-to-edge wrapping mode (`GL_CLAMP_TO_EDGE`), where texture coordinates outside [0, 1] are clamped to the edges of the texture.
	};
}