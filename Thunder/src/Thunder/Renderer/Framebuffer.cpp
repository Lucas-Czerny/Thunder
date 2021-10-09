#include "td_pch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Thunder
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& properties)
	{
		switch (Renderer::GetRenderingAPI())
		{
			case RenderingAPI::None:
			{
				TD_CORE_ASSERT(false, "RendererAPI is not set (RendererAPI::None)");
				return nullptr;
			}
			case RenderingAPI::OpenGL:
			{
				return CreateRef<OpenGLFramebuffer>(properties);
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}