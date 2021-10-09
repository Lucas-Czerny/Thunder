#include "td_pch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Thunder
{
	uint32_t ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		TD_CORE_ASSERT(false, "Unknown Shader Data Type");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		TD_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		TD_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		TD_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		TD_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> vertexBuffer)
	{
		TD_PROFILE_FUNCTION();

		TD_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Layout is not set");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		// Create Layout

		const BufferLayout& layout = vertexBuffer->GetLayout();

		std::vector<BufferElement> elements = layout.GetElements();
		for (uint32_t i = 0; i < elements.size(); i++)
		{
			BufferElement element = elements[i];

			
			switch (element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(i);

					glVertexAttribPointer(
						i,
						element.Count,
						ShaderDataTypeToOpenGLType(element.Type),
						element.Normalized,
						layout.GetStride(),
						(const void*)element.Offset
					);

					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(i);

					glVertexAttribIPointer(i,
						element.Count,
						GL_INT,
						layout.GetStride(),
						(const void*)element.Offset);

					break;
				}
				default:
				{
					TD_CORE_ASSERT(false, "Shader Data Type not supported yet :o");

					break;
				}
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
