#pragma once
#include<GL/glew.h>
#include"Texture.h"
#include<iostream>
#include"CommonAppData.h"

namespace OBJ_Viewer {

	enum FramebufferAttachmentsFlags
	{
		FRAMEBUFFER_COLOR_ATTACHMENT = 1,
		FRMAEBUFFER_DEPTH_STENCIL_ATTACHMENT = 2,
		FRAMEBUFFER_NORMAL_ATTACHMENT = 4,
		FRAMEBUFFER_SPECULAR_ATTACHMENT = 8
	};

	class Framebuffer
	{
	public:
		//FramebufferAttachmentsFlags currently not used
		Framebuffer(Size2D size, FramebufferAttachmentsFlags attachmentFlags,bool isMultiSampleBuffer = false,uint8_t sampleCount = 4);
		GLuint GetFramebufferHandle()const { return this->m_framebuffer; }
		Texture& GetFramebufferTexture()const { return *this->m_texture; }
		void BindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer); }
		static void BindDefaultFramebuffer() {  glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		void UnbindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		bool isFramebufferValid()const;
		Size2D GetFramebufferSize()const { return m_framebufferSize; }
		void ResizeFramebuffer(Size2D newSize);
		void CopyFramebufferContent(const Framebuffer& framebufferToCopy);
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

