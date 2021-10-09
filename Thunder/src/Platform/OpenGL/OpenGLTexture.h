#pragma once

#include <glad/glad.h>
#include "Thunder/Renderer/Texture.h"

namespace Thunder
{
	struct OpenGLTextureProperties
	{
		float TilingFactor = 1.0f;
		glm::vec4 Tint = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLenum InternalFormat, DataFormat;
		GLenum TextureMinFilter = GL_NEAREST, TextureMagFilter = GL_NEAREST;
		GLenum TextureWrapS = GL_CLAMP_TO_EDGE, TextureWrapT = GL_CLAMP_TO_EDGE;
	};

	class OpenGLTexture2D : public Texture2D
	{
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		TextureProperties m_TextureProperties;

	public:
		OpenGLTexture2D(const fs::path& path, const TextureProperties& properties);
		OpenGLTexture2D(uint32_t width, uint32_t height, const TextureProperties& properties, void* data, uint32_t size);
		~OpenGLTexture2D();

		inline int32_t GetRendererID() const override { return m_RendererID; }

		inline int32_t GetWidth() const override { return m_Width; }
		inline int32_t GetHeight() const override { return m_Height; }

		void SetData(void* data, uint32_t size) override;
		void SetTextureProperties(const TextureProperties& properties) override { m_TextureProperties = properties; }
		const TextureProperties& GetTextureProperties() override { return m_TextureProperties; };

		void Bind(uint32_t slot = 0) const override;

		bool operator==(const Texture2D& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	};
}


