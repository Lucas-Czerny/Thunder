#include "td_pch.h"
#include "SubTexture2D.h"

namespace Thunder
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2 texCoords[4])
		: m_Texture(texture)
	{
		for (int i = 0; i < 4; i++)
		{
			m_TexCoords[i] = texCoords[i];
		}
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& subTextureSize /* = { 1, 1 } */)
	{
		glm::vec2 min = coords;
		glm::vec2 max = { coords.x + subTextureSize.x, coords.y + subTextureSize.y };

		float xMultiplier = spriteSize.x / texture->GetWidth();
		float yMultiplier = spriteSize.y / texture->GetHeight();

		glm::vec2 texCoords[4] = {
			{ min.x * xMultiplier, min.y * yMultiplier },
			{ max.x * xMultiplier, min.y * yMultiplier },
			{ max.x * xMultiplier, max.y * yMultiplier },
			{ min.x * xMultiplier, max.y * yMultiplier }
		};

		return CreateRef<SubTexture2D>(texture, texCoords);
	}
}
