#include "Thunder/Renderer/VertexArray.h"
#include "Thunder/Renderer/Shader.h"
#include "Thunder/Renderer/Camera.h"

#include "Thunder/Renderer/Texture.h"
#include "Thunder/Renderer/SubTexture2D.h"

#include "Thunder/Scene/EditorCamera.h"
#include "Thunder/Scene/Components.h"

namespace Thunder
{
	class Renderer2D
	{
	private:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoords;
			float TexIndex;
			float TilingFactor;
			int EntityID;
		};

		struct Renderer2DData
		{
			// TODO: Driver Capability
			const uint32_t MaxQuads = 10000;
			const uint32_t MaxVertices = MaxQuads * 4;
			const uint32_t MaxIndices = MaxQuads * 6;
			static const uint32_t MaxTextureSlots = 32;

			uint32_t QuadIndexCount = 0;

			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPointer = nullptr;

			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<Shader> SingleShader;

			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32_t CurrentTextureSlotIndex = 1;

			glm::vec4 QuadVertexPositions[4];
			glm::vec2 DefaultTextureCoordinates[4] = {
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f }
			};
		};

		static Renderer2DData s_Data;

		struct Renderer2DStats
		{
			uint32_t DrawCalls = 0, QuadCount = 0;
		};

		static Renderer2DStats s_Stats;

	public:
		static void Init();
		static void Shutdown();

		static Ref<Texture2D> WhiteTexture() { return Texture2D::WhiteTexture; }

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		// ECS

		static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRendererComponent, int entityID = -1);

		// With tranform matrix

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, int entityID = -1);

		// Not Rotated

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);

		// Rotated
		// Rotation in Radians

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture);

		static void SetVertexBufferData(const glm::mat4& transform, const glm::vec4& color, float textureIndex, float tilingFactor, int entityID = -1, const glm::vec2 textureCoords[4] = s_Data.DefaultTextureCoordinates);

		static Renderer2DStats GetStats() { return s_Stats; }
		static void ResetStats();

	private:
		static void Reset();
		static void Flush();
	};
}