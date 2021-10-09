#include "td_pch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Thunder
{
	Ref<VertexArray> VertexArray::Create()
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
				return CreateRef<OpenGLVertexArray>();
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}