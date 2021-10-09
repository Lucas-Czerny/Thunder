#include "td_pch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Thunder
{
	Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, uint32_t size, bool dynamic /* = false */)
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
				return CreateRef<OpenGLVertexBuffer>(vertices, size, dynamic);
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	
	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t size, bool dynamic /* = false */)
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
				return CreateRef<OpenGLIndexBuffer>(indices, size, dynamic);
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
