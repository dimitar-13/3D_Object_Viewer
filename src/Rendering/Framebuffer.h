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
		Framebuffer(int width, int height, FramebufferAttachmentsFlags attachmentFlags);
		GLuint GetFramebufferHandle()const { return this->m_framebuffer; }
		Texture& GetFramebufferTexture()const { return *this->m_texture; }
		void BindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer); }
		void UnbindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		bool isFramebufferValid()const;
		Size2D GetFramebufferSize()const { return { m_framebufferSize.width ,m_framebufferSize.height}; }
		void ResizeFramebuffer(int newWidth, int newHeight);
		~Framebuffer();
	private:
		GLuint m_framebuffer;
		GLuint m_readBuffer;
		Size2D m_framebufferSize;
		std::unique_ptr<Texture> m_texture;
	};
}

