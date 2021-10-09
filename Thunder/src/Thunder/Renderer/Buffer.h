#pragma once

namespace Thunder
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	struct BufferElement
	{
	public:
		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		uint32_t Count;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Type(type), Name(name), Size(GetDataTypeSize()), Count(GetCount()), Offset(0), Normalized(normalized) {}

	private:
		uint32_t GetDataTypeSize() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 4;
				case ShaderDataType::Float2:	return 4 * 2;
				case ShaderDataType::Float3:	return 4 * 3;
				case ShaderDataType::Float4:	return 4 * 4;
				case ShaderDataType::Mat3:		return 4 * 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4 * 4;
				case ShaderDataType::Int:		return 4;
				case ShaderDataType::Int2:		return 4 * 2;
				case ShaderDataType::Int3:		return 4 * 3;
				case ShaderDataType::Int4:		return 4 * 4;
				case ShaderDataType::Bool:		return 1;
			}

			TD_CORE_ASSERT(false, "Unknown Shader Data Type");
			return 0;
		}

		uint32_t GetCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			TD_CORE_ASSERT(false, "Unknown Shader Data Type");
			return 0;
		}
	};

	struct BufferLayout
	{
	private:
		uint32_t m_Stride = 0;
		std::vector<BufferElement> m_Elements;

	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			for (BufferElement& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline std::vector<BufferElement> GetElements() const { return m_Elements; }
	};

	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(const float* vertices, uint32_t size, bool dynamic = false);
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const float* vertices, uint32_t size, uint32_t offset = 0) = 0;

		virtual void SetLayout(BufferLayout& layout) = 0;
		virtual BufferLayout GetLayout() const = 0;
	};
	
	class IndexBuffer
	{
		public:
		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t size, bool dynamic = false);
		virtual ~IndexBuffer() {}

		virtual inline uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const uint32_t* indices, uint32_t size, uint32_t offset = 0) = 0;
	};
}