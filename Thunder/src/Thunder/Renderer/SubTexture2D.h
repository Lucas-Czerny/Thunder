#pragma once

#include "Thunder/Renderer/Texture.h"

namespace Thunder
{
	class SubTexture2D
	{
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];

	public:	
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2 texCoords[4]);

		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& spriteSize, const glm::vec2& subTextureSize = { 1, 1 });
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		Ref<Texture2D> GetTexture() const { return m_Texture; }
	};
}


