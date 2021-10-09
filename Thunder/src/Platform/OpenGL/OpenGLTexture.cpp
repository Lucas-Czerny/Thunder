#include "td_pch.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Thunder
{
	OpenGLTexture2D::OpenGLTexture2D(const fs::path& path, const TextureProperties& properties)
		: m_TextureProperties(properties)
	{
		std::string pathString = path.string();

		TD_PROFILE_FUNCTION();

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;

		{
			TD_PROFILE_SCOPE("stbi_load in OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
			data = stbi_load(pathString.c_str(), &width, &height, &channels, NULL);
		}

		TD_CORE_ASSERT(data, "Failed to load image from path \"{0}\"", pathString);

		m_Width = width;
		m_Height = height;

		if (channels == 3)
		{
			m_TextureProperties.InternalFormat = GL_RGB8;
			m_TextureProperties.DataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			m_TextureProperties.InternalFormat = GL_RGBA8;
			m_TextureProperties.DataFormat = GL_RGBA;
		}
		else
		{
			TD_CORE_ASSERT(false, "Texture format not supported!");
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_TextureProperties.InternalFormat, m_Width, m_Height);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureProperties.TextureMinFilter);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureProperties.TextureMagFilter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureProperties.TextureWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureProperties.TextureWrapT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_TextureProperties.DataFormat, GL_UNSIGNED_BYTE, data);

		if (data)
		{
			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const TextureProperties& properties, void* data, uint32_t size)
		: m_Width(width), m_Height(height)
	{
		TD_PROFILE_FUNCTION();

		m_TextureProperties.InternalFormat = GL_RGBA8;
		m_TextureProperties.DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1,  m_TextureProperties.InternalFormat, m_Width, m_Height);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, properties.TextureMinFilter);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, properties.TextureMagFilter);

		SetData(data, size);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		TD_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		TD_PROFILE_FUNCTION();

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height,  m_TextureProperties.DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot /* = 0 */) const
	{
		TD_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}