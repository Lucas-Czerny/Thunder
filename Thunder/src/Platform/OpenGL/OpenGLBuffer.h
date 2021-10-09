#pragma once

#include "Thunder/Renderer/Buffer.h"

namespace Thunder
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;

	public:
		OpenGLVertexBuffer(const float* vertices, uint32_t size, bool dynamic);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const float* vertices, uint32_t size, uint32_t offset = 0) override;

		void SetLayout(BufferLayout& layout) override;
		BufferLayout GetLayout() const override { return m_Layout; }
	};
	
	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;

	public:
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t size, bool dynamic);
		~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const uint32_t* indices, uint32_t size, uint32_t offset = 0) override;

		inline uint32_t GetCount() const { return m_Count; };
	};
}

