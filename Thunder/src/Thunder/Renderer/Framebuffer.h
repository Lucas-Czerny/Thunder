#pragma once

namespace Thunder
{
	enum class FramebufferFormat
	{
		None = 0,

		// Color
		RGBA8, R32I,

		// Depth / Stencil
		DEPTH24STENCIL8,

		// Default Depth Format
		DEPTH = DEPTH24STENCIL8
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferFormat Format = FramebufferFormat::None;
		// TODO: filtering / wrap

		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(FramebufferFormat format)
			: Format(format) {}
	};

	typedef std::vector<FramebufferAttachmentSpecification> FramebufferAttachments;

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		FramebufferAttachments Attachments;

		// Rendering to screen ... true
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpecification& properties);

		virtual ~Framebuffer() = default;

		virtual void Recreate() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void ClearAttachment(uint32_t attachmentID, int value) const = 0;
		virtual void ClearAttachment(uint32_t attachmentID, float value) const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual int ReadPixel(int x, int y, uint32_t attachmentIndex = 0) const = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
	};
}
