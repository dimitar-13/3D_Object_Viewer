#include "Framebuffer.h"
#include<iostream>
OBJ_Viewer::Framebuffer::Framebuffer(int width, int height, FramebufferAttachmentsFlags attachmentFlags)
{
	m_Width = width;
	m_Height = height;

	glGenFramebuffers(1, &this->m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer);

	TextureBuilder builder;
	this->m_texture = 
		builder.SetTextureFormat(TEXTURE_FORMAT_RGB).
		SetTextureInternalFormat(TEXTURE_INTERNAL_FORMAT_RGB).
		SetTextureSize({ width,height }).buildTexture();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,this->m_texture->GetTextureHandle(), 0);
	glGenRenderbuffers(1, &this->m_readBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_readBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_readBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR]:Failed to create valid framebuffer." << '\n';
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool OBJ_Viewer::Framebuffer::isFramebufferValid() const
{
	BindFramebuffer();
	const bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
	UnbindFramebuffer();
	return result;
}

void OBJ_Viewer::Framebuffer::ResizeFramebuffer(int newWidth, int newHeight)
{
	if (m_Width == newWidth && m_Height == newHeight)
		return;

	m_Width = newWidth;
	m_Height = newHeight;

	BindFramebuffer();
	this->m_texture->ResizeTexture({ newWidth,newHeight });
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_texture->GetTextureHandle(), 0);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_readBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_readBuffer);

	UnbindFramebuffer();
}

OBJ_Viewer::Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &this->m_framebuffer);
	glDeleteRenderbuffers(1, &this->m_readBuffer);
}

