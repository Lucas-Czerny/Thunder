#include "td_pch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Thunder
{
	RenderingAPI Renderer::s_RenderingAPI = RenderingAPI::OpenGL;
	RendererAPI* Renderer::s_RendererAPI = new OpenGLRendererAPI();
	glm::mat4 Renderer::m_ViewProjectionMatrix;

	void Renderer::Init()
	{
		s_RendererAPI->Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(WindowResizeEvent& event)
	{
		SetViewport(0, 0, event.GetWidth(), event.GetHeight());
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelMatrix)
	{
		vertexArray->Bind();
		shader->Bind();

		shader->SetMat4("u_ViewProjectionMatrix", m_ViewProjectionMatrix);
		shader->SetMat4("u_ModelMatrix", modelMatrix);

		DrawElement(vertexArray);
	}

	void Renderer::DrawElement(const Ref<VertexArray>& vertexArray, uint32_t indexCount /* = 0 */)
	{
		s_RendererAPI->DrawElement(vertexArray, indexCount);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void Renderer::ClearColor()
	{
		s_RendererAPI->ClearColor();
	}

	void Renderer::SetViewport(float x, float y, float width, float height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
	}
}