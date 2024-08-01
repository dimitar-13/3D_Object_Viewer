#pragma once
#include "pch.h"
#include "Texture.h"
#include "Core/CommonAppData.h"

namespace OBJ_Viewer {

	enum FramebufferAttachmentsFlags_
	{
		FramebufferAttachmentsFlags_kColorAttachment = 1,
		FramebufferAttachmentsFlags_kStencilAttachment = 2,
		//FRAMEBUFFER_NORMAL_ATTACHMENT = 4,
		//FRAMEBUFFER_SPECULAR_ATTACHMENT = 8
	};
    enum FramebufferBitMaskFlags_
    {
        FramebufferBitMaskFlags_kColorBufferBit = GL_COLOR_BUFFER_BIT,
        FramebufferBitMaskFlags_kDepthBufferBit = GL_DEPTH_BUFFER_BIT
    };

	class Framebuffer
	{
	public:
		using pixel_component = unsigned char;
		//FramebufferAttachmentsFlags currently not used
		Framebuffer(Size2D size, FramebufferAttachmentsFlags_ attachmentFlags,bool isMultiSampleBuffer = false,uint8_t sampleCount = 4);
		GLuint GetFramebufferHandle()const { return this->m_framebuffer; }
		Texture& GetFramebufferTexture()const { return *this->m_texture; }
		void BindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer); }
		static void BindDefaultFramebuffer() {  glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		void UnbindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		bool isFramebufferValid()const;
		Size2D GetFramebufferSize()const { return m_framebufferSize; }
		void ResizeFramebuffer(Size2D newSize);
		void CopyFramebufferContent(const Framebuffer& framebufferToCopy, FramebufferBitMaskFlags_ mask = FramebufferBitMaskFlags_kColorBufferBit);
		std::vector<pixel_component> GetFramebufferPixels(TextureFormat_ retrieveFormat)const;
		~Framebuffer();
	private:
		GLuint m_framebuffer;
		GLuint m_readBuffer;
		Size2D m_framebufferSize;
		bool m_isMultiSample = false;
		uint8_t m_sampleCount = 4;
		std::unique_ptr<Texture> m_texture;
	};
}

