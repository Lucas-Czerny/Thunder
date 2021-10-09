#pragma once

#include "Thunder/Renderer/Framebuffer.h"

typedef unsigned int GLenum;

namespace Thunder
{
	struct GLTextureFormats
	{
		GLenum internalFormat, accessFormat;
	};

	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferAttachmentSpecification> m_ColorAttachments;
		FramebufferAttachmentSpecification m_DepthAttachment = {};

		std::vector<uint32_t> m_ColorAttachmentIDs;
		uint32_t m_DepthAttachmentID;

	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		~OpenGLFramebuffer();

		void Recreate() override;

		void Bind() const override;
		void Unbind() const override;

		void ClearAttachment(uint32_t attachmentID, int value) const;
		void ClearAttachment(uint32_t attachmentID, float value) const;

		void Resize(uint32_t width, uint32_t height) override;

		virtual int ReadPixel(int x, int y, uint32_t attachmentIndex = 0) const override;

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;
		const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		static bool IsDepthBuffer(FramebufferFormat format);
		static void CreateTextures(uint32_t* outID, uint32_t count);
		static GLTextureFormats GetGLTextureFormats(FramebufferFormat format);

		void AttachTexture(uint32_t id, const FramebufferAttachmentSpecification& attachment, int attachmentSlot);
		void DeleteAll();
	};
}


