#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Thunder
{
	enum class RenderingAPI
	{
		None = 0,
		OpenGL, Vulkan
	};

	class RendererAPI
	{
	public:
		virtual void Init() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void ClearColor() = 0;

		virtual void DrawElement(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		virtual void SetViewport(float x, float y, float width, float height) = 0;
	};
}


