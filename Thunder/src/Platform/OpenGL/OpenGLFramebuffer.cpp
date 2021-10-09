#include "td_pch.h"	
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Thunder
{
	bool OpenGLFramebuffer::IsDepthBuffer(FramebufferFormat format)
	{
		switch(format)
		{
			case FramebufferFormat::DEPTH24STENCIL8:
			{
				return true;
			}
		}

		return false;
	}

	void OpenGLFramebuffer::CreateTextures(uint32_t* outID, uint32_t count)
	{
		glCreateTextures(GL_TEXTURE_2D, count, outID);
	}

	GLTextureFormats OpenGLFramebuffer::GetGLTextureFormats(FramebufferFormat format)
	{
		static std::unordered_map<FramebufferFormat, GLTextureFormats> map = {
			{ FramebufferFormat::RGBA8, { GL_RGBA8, GL_RGBA } },
			{ FramebufferFormat::R32I, { GL_R32I, GL_RED_INTEGER } },
			{ FramebufferFormat::DEPTH24STENCIL8, { GL_DEPTH24_STENCIL8, 0 } },
		};

		return map[format];
	}


	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
		: m_Specification(specification)
	{
		for (auto attachment : m_Specification.Attachments)
		{
			if (IsDepthBuffer(attachment.Format))
				m_DepthAttachment = attachment;
			else
				m_ColorAttachments.emplace_back(attachment);
		}
		
		Recreate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		DeleteAll();
	}

	void OpenGLFramebuffer::Recreate()
	{
		if (m_RendererID)
		{
			DeleteAll();			
		}

		glCreateFramebuffers(1, &m_RendererID);

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		uint32_t size = m_ColorAttachments.size();
		TD_CORE_ASSERT(size <= 4, "Thunder does not support more than 4 Color Attachments :(");

		if (size != 0)
		{
			m_ColorAttachmentIDs.resize(size);
			CreateTextures(m_ColorAttachmentIDs.data(), size);

			for (int i = 0; i < size; i++)
			{
				FramebufferAttachmentSpecification attachment = m_ColorAttachments[i];
				uint32_t id = m_ColorAttachmentIDs[i];

				AttachTexture(id, attachment, GL_COLOR_ATTACHMENT0 + i);
			}

			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(size, buffers);
		}
		else
		{
			glDrawBuffer(GL_NONE);
		}

		if (m_DepthAttachment.Format != FramebufferFormat::None)
		{
			uint32_t id;
			glCreateTextures(GL_TEXTURE_2D, 1, &id);

			AttachTexture(id, m_DepthAttachment, GL_DEPTH_ATTACHMENT);
		}

		TD_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::AttachTexture(uint32_t id, const FramebufferAttachmentSpecification& attachment, int attachmentSlot)
	{
		glBindTexture(GL_TEXTURE_2D, id);

		GLTextureFormats formats = GetGLTextureFormats(attachment.Format);

		if (attachmentSlot == GL_DEPTH_ATTACHMENT)
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, formats.internalFormat, m_Specification.Width, m_Specification.Height);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, formats.internalFormat, m_Specification.Width, m_Specification.Height,
				0, formats.accessFormat, GL_UNSIGNED_BYTE, nullptr);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentSlot, GL_TEXTURE_2D, id, 0);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentID, float value) const
	{
		TD_CORE_ASSERT(index < m_ColorAttachmentIDs.size(), "Index out of range");

		FramebufferFormat format = m_ColorAttachments[attachmentID].Format;
		GLenum accessFormat = GetGLTextureFormats(format).accessFormat;

		glClearTexImage(m_ColorAttachmentIDs[attachmentID], 0, accessFormat, GL_FLOAT, &value);
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentID, int value) const
	{
		TD_CORE_ASSERT(index < m_ColorAttachmentIDs.size(), "Index out of range");

		FramebufferFormat format = m_ColorAttachments[attachmentID].Format;
		GLenum accessFormat = GetGLTextureFormats(format).accessFormat;

		glClearTexImage(m_ColorAttachmentIDs[attachmentID], 0, accessFormat, GL_INT, &value);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;

		Recreate();
	}

	void OpenGLFramebuffer::DeleteAll()
	{
		glDeleteFramebuffers(1, &m_RendererID);

		glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
		glDeleteTextures(1, &m_DepthAttachmentID);
	}

	int OpenGLFramebuffer::ReadPixel(int x, int y, uint32_t attachmentIndex /* = 0 */) const
	{
		TD_CORE_ASSERT(index < m_ColorAttachmentIDs.size(), "AttachmentIndex is too large!");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

		int result;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &result);

		return result;
	}

	uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index /* = 0 */) const
	{
		TD_CORE_ASSERT(index < m_ColorAttachmentIDs.size(), "Index out of range");
		return m_ColorAttachmentIDs[index];
	}
}
