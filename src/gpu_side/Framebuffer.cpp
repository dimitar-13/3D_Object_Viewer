#include "pch.h"
#include "Framebuffer.h"
#include"Logging/App_Logger.h"

constexpr uint8_t FRAMEBUFFER_TEXTURE_COMPONENT_COUNT = 4;

OBJ_Viewer::Framebuffer::Framebuffer(Size2D size, FramebufferAttachmentsFlags attachmentFlags, bool isMultiSampleBuffer, uint8_t sampleCount):
m_framebufferSize(size),m_isMultiSample(isMultiSampleBuffer),m_sampleCount(sampleCount)
{
	glGenFramebuffers(1, &this->m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer);

	TextureBuilder builder;
	this->m_texture = 
		builder.SetTextureFormat(TEXTURE_FORMAT_RGBA).
		SetTextureInternalFormat(TEXTURE_INTERNAL_FORMAT_RGBA).
		SetTextureSize(size).isTextureMultiSample(m_isMultiSample).SetSampleCount(sampleCount)
		.buildTexture();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_isMultiSample? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
		this->m_texture->GetTextureHandle(), 0);

#pragma region Setup render buffer object


	glGenRenderbuffers(1, &this->m_readBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_readBuffer);
	if (!m_isMultiSample) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.width, size.height);
	}
	else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_DEPTH24_STENCIL8, size.width, size.height);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_readBuffer);
#pragma endregion

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOGGER_LOG_ERROR("Failed to create valid framebuffer.");
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

void OBJ_Viewer::Framebuffer::ResizeFramebuffer(Size2D newSize)
{
	m_framebufferSize = newSize;

	BindFramebuffer();
	this->m_texture->ResizeTexture(m_framebufferSize);

	glBindRenderbuffer(GL_RENDERBUFFER, this->m_readBuffer);
	if (!m_isMultiSample) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_framebufferSize.width, m_framebufferSize.height);
	}
	else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_DEPTH24_STENCIL8, m_framebufferSize.width, m_framebufferSize.height);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_readBuffer);

	UnbindFramebuffer();

}

void OBJ_Viewer::Framebuffer::CopyFramebufferContent(const Framebuffer& framebufferToCopy)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferToCopy.m_framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
	glBlitFramebuffer(0, 0, m_framebufferSize.width, m_framebufferSize.height, 0, 0,
		m_framebufferSize.width, m_framebufferSize.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

std::vector<OBJ_Viewer::Framebuffer::pixel_component> OBJ_Viewer::Framebuffer::GetFramebufferPixels(TextureFormat retrieveFormat)
{
	const size_t FRAMEBUFFER_TEXTURE_BUFFER_SIZE = m_framebufferSize.width * m_framebufferSize.height * FRAMEBUFFER_TEXTURE_COMPONENT_COUNT;
	std::vector<pixel_component> result(FRAMEBUFFER_TEXTURE_BUFFER_SIZE);
	BindFramebuffer();

	glReadPixels(0, 0, m_framebufferSize.width, m_framebufferSize.height, retrieveFormat, GL_UNSIGNED_BYTE, result.data());
	UnbindFramebuffer();

	return result;
}

OBJ_Viewer::Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &this->m_framebuffer);
	glDeleteRenderbuffers(1, &this->m_readBuffer);
}

