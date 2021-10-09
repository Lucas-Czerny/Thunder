#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RendererAPI.h"

#include "Thunder/Events/ApplicationEvent.h"

#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Thunder
{
	class Renderer
	{
	private:
		static RenderingAPI s_RenderingAPI;
		static RendererAPI* s_RendererAPI;

		static glm::mat4 m_ViewProjectionMatrix;

	public:
		static void Init();
		static void OnWindowResize(WindowResizeEvent& event);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelMatrix);

		static void DrawElement(const Ref<VertexArray>& vertexArray, uint32_t indexCounter = 0);

		static void SetClearColor(const glm::vec4& color);
		static void ClearColor();

		static void SetViewport(float x, float y, float width, float height);

		inline static RenderingAPI GetRenderingAPI() { return s_RenderingAPI; }
	};
}