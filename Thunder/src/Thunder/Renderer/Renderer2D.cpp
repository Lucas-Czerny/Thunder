#include "td_pch.h"
#include "Renderer2D.h"

#include "Thunder/Renderer/Renderer.h"

namespace Thunder
{
	Renderer2D::Renderer2DData Renderer2D::s_Data;
	Renderer2D::Renderer2DStats Renderer2D::s_Stats;

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(nullptr, s_Data.MaxVertices * sizeof(QuadVertex), true);

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
		};

		s_Data.QuadVertexBuffer->SetLayout(layout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* indices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices, s_Data.MaxIndices * sizeof(uint32_t));
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] indices;

		s_Data.SingleShader = Shader::Create("resources/shaders/single.glsl");

		// Create White Texture
		uint32_t whiteTextureData = 0xffffffff;
		Texture2D::WhiteTexture = Texture2D::Create(1, 1, &whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots[0] = Texture2D::WhiteTexture;

		s_Data.SingleShader->Bind();

		int sampler2DUniform[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			sampler2DUniform[i] = i;
		}

		s_Data.SingleShader->SetIntArray("u_Textures", sampler2DUniform, s_Data.MaxTextureSlots);

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		TD_PROFILE_FUNCTION();

		s_Data.SingleShader->Bind();
		s_Data.SingleShader->SetMat4("u_ViewProjectionMatrix", camera.GetProjectionMatrix() * glm::inverse(transform));

		Reset();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		TD_PROFILE_FUNCTION();

		s_Data.SingleShader->Bind();
		s_Data.SingleShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		Reset();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Reset()
	{
		s_Data.QuadVertexBufferPointer = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;
		s_Data.CurrentTextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		TD_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.CurrentTextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPointer - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData((const float*)s_Data.QuadVertexBufferBase, dataSize);

		s_Data.SingleShader->Bind();
		Renderer::DrawElement(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		s_Stats.DrawCalls++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRendererComponent, int entityID /* = -1 */)
	{
		TD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
		{
			Flush();
			Reset();
		}

		glm::vec4 color = spriteRendererComponent.Color;
		Ref<Texture2D> texture = spriteRendererComponent.Texture;

		auto textureProperties = texture->GetTextureProperties();

		float tilingFactor = textureProperties.TilingFactor;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.CurrentTextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.CurrentTextureSlotIndex;
			s_Data.TextureSlots[s_Data.CurrentTextureSlotIndex] = texture;
			s_Data.CurrentTextureSlotIndex++;
		}

		SetVertexBufferData(transform, color, textureIndex, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID /* = -1 */)
	{
		TD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
		{
			Flush();
			Reset();
		}

		float whiteTextureIndex = 0.0f;
		float tilingFactor = 1.0f;

		SetVertexBufferData(transform, color, whiteTextureIndex, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int entityID /* = -1 */)
	{
		TD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
		{
			Flush();
			Reset();
		}

		auto textureProperties = texture->GetTextureProperties();

		const glm::vec4 color = textureProperties.Tint;
		float tilingFactor = textureProperties.TilingFactor;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.CurrentTextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.CurrentTextureSlotIndex;
			s_Data.TextureSlots[s_Data.CurrentTextureSlotIndex] = texture;
			s_Data.CurrentTextureSlotIndex++;
		}

		SetVertexBufferData(transform, color, textureIndex, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, int entityID /* = -1 */)
	{
		TD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
		{
			Flush();
			Reset();
		}

		const glm::vec2* texCoords = subTexture->GetTexCoords();
		Ref<Texture2D> texture = subTexture->GetTexture();

		auto textureProperties = texture->GetTextureProperties();

		const glm::vec4 color = textureProperties.Tint;
		float tilingFactor = textureProperties.TilingFactor;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.CurrentTextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.CurrentTextureSlotIndex;
			s_Data.TextureSlots[s_Data.CurrentTextureSlotIndex] = texture;
			s_Data.CurrentTextureSlotIndex++;
		}

		SetVertexBufferData(transform, color, textureIndex, tilingFactor, entityID, texCoords);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture);
	}

	void Renderer2D::SetVertexBufferData(const glm::mat4& transform, const glm::vec4& color, float textureIndex, float tilingFactor, int entityID /* = -1 */, const glm::vec2 textureCoords[4] /* = s_Data.DefaultTextureCoordinates */)
	{
		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPointer->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPointer->Color = color;
			s_Data.QuadVertexBufferPointer->TexCoords = textureCoords[i];
			s_Data.QuadVertexBufferPointer->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPointer->EntityID = entityID;

			s_Data.QuadVertexBufferPointer++;
		}

		s_Data.QuadIndexCount += 6;
		s_Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Stats, 0, sizeof(Renderer2DStats));
	}
}