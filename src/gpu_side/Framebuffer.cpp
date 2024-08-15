#include "pch.h"
#include "Framebuffer.h"
#include"Logging/App_Logger.h"
#include "Helpers/TextureHelpers.h"
constexpr size_t FRAMEBUFFER_TEXTURE_COMPONENT_COUNT = 4;

OBJ_Viewer::Framebuffer::Framebuffer(Size2D size, FramebufferAttachmentsFlags_ attachmentFlags, bool isMultiSampleBuffer, uint8_t sampleCount):
m_FramebufferSize(size),m_IsMultiSample(isMultiSampleBuffer),m_SampleCount(sampleCount)
{
	glGenFramebuffers(1, &this->m_Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_Framebuffer);

	TextureBuilder builder;
	this->m_FramebufferColorAttachmentTexture = 
		builder.SetTextureFormat(TextureFormat_kRGBA).
		SetTextureInternalFormat(TextureInternalFormat_kRGBA).
		SetTextureSize(size).isTextureMultiSample(m_IsMultiSample).SetSampleCount(sampleCount)
		.buildTexture();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_IsMultiSample? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
		this->m_FramebufferColorAttachmentTexture->GetTextureHandle(), 0);

#pragma region Setup render buffer object


	glGenRenderbuffers(1, &this->m_ReadBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_ReadBuffer);
	if (!m_IsMultiSample) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.width, size.height);
	}
	else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_SampleCount, GL_DEPTH24_STENCIL8, size.width, size.height);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_ReadBuffer);
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
	if (newSize.width == m_FramebufferSize.width && newSize.height == m_FramebufferSize.height)
		return;

	m_FramebufferSize = newSize;

	BindFramebuffer();
	this->m_FramebufferColorAttachmentTexture->ResizeTexture(m_FramebufferSize);

	glBindRenderbuffer(GL_RENDERBUFFER, this->m_ReadBuffer);
	if (!m_IsMultiSample) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_FramebufferSize.width, m_FramebufferSize.height);
	}
	else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_SampleCount, GL_DEPTH24_STENCIL8, m_FramebufferSize.width, m_FramebufferSize.height);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_ReadBuffer);

	UnbindFramebuffer();

}

void OBJ_Viewer::Framebuffer::CopyFramebufferContent(const Framebuffer& framebufferToCopy, FramebufferBitMaskFlags_ mask)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferToCopy.m_Framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer);
	glBlitFramebuffer(0, 0, m_FramebufferSize.width, m_FramebufferSize.height, 0, 0,
		m_FramebufferSize.width, m_FramebufferSize.height, mask, GL_NEAREST);
}

std::vector<OBJ_Viewer::Framebuffer::pixel_component> OBJ_Viewer::Framebuffer::GetFramebufferPixels(TextureFormat_ retrieveFormat)const
{
	const size_t kFramebufferTextureByteSize = 
        static_cast<size_t>(m_FramebufferSize.width * m_FramebufferSize.height *
            TextureFormatEnumConverter::GetChannelCountByFormat(retrieveFormat));

	std::vector<pixel_component> result(kFramebufferTextureByteSize);
	BindFramebuffer();
	glReadPixels(0, 0, m_FramebufferSize.width, m_FramebufferSize.height, retrieveFormat, GL_UNSIGNED_BYTE, result.data());
	UnbindFramebuffer();
	return result;
}

OBJ_Viewer::Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &this->m_Framebuffer);
	glDeleteRenderbuffers(1, &this->m_ReadBuffer);
}

