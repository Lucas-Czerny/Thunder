#include "td_pch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Thunder
{
	// Create White Texture
	Ref<Texture2D> Texture2D::WhiteTexture;

	TextureProperties TextureProperties::Create()
	{
		switch (Renderer::GetRenderingAPI())
		{
			case RenderingAPI::None:
			{
				TD_CORE_ASSERT(false, "RendererAPI is not set (RendererAPI::None)");
			}
			case RenderingAPI::OpenGL:
			{
				return (TextureProperties&)OpenGLTextureProperties();
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
	}

	Ref<Texture2D> Texture2D::Create(const fs::path& path, const TextureProperties& properties /* = TextureProperties() */)
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
				return CreateRef<OpenGLTexture2D>(path, properties);
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, const TextureProperties& properties, void* data /* = nullptr */, uint32_t size /* = 0 */)
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
				return CreateRef<OpenGLTexture2D>(width, height, properties, data, size);
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data /* = nullptr */, uint32_t size /* = 0 */, const TextureProperties& properties /* = TextureProperties() */)
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
				return CreateRef<OpenGLTexture2D>(width, height, properties, data, size);
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}