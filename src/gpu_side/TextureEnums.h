#pragma once
#include "pch.h"
namespace OBJ_Viewer {
	enum TextureFormat_
	{
		TextureFormat_kUnknown,
		TextureFormat_kR = GL_RED,
		TextureFormat_kRG = GL_RG,
		TextureFormat_kRGB = GL_RGB,
		TextureFormat_kRGBA = GL_RGBA
	};
	enum TextureInternalFormat_ {
		TextureInternalFormat_kDepth = GL_DEPTH_COMPONENT,
		TextureInternalFormat_kDepth_and_stencil = GL_DEPTH_STENCIL,
		TextureInternalFormat_kR = GL_RED,
		TextureInternalFormat_kRG = GL_RG,
		TextureInternalFormat_kRGB = GL_RGB,
		TextureInternalFormat_kRGBA = GL_RGBA,
	};
	enum TextureFilter_
	{
		TextureFilter_kLinear = GL_LINEAR,
		TextureFilter_kNearest = GL_NEAREST
	};
	enum TextureWrap_
	{
		TextureWrap_kRepeat = GL_REPEAT,
		TextureWrap_kMirroredRepeat = GL_MIRRORED_REPEAT,
		TextureWrap_kClampToEdge = GL_CLAMP_TO_EDGE,
	};
}