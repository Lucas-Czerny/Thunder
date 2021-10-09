#pragma once

#include "Thunder/Renderer/VertexArray.h"

namespace Thunder
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexBufferIndex = 0;

	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const Ref<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer> indexBuffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	};
}


