#pragma once

#include "Thunder/Renderer/RendererAPI.h"

namespace Thunder
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;

		void SetClearColor(const glm::vec4& color) override;
		void ClearColor() override;

		void DrawElement(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

		void SetViewport(float x, float y, float width, float height) override;
	};
}
