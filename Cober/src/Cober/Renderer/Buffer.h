#pragma once

namespace Cober {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4,	Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) 
	{
		switch (type) 
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

		CB_LogInfo(LOG_RENDER, "Unknown Shader Data Type!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		uint32_t GetComponentCount() const {

			switch (Type) {

				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int4:		return 3;
				case ShaderDataType::Int3:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			CB_LogInfo(LOG_RENDER, "Unwknown Shader Type!");
			return 0;
		}
	};

	class BufferLayout 
	{
	public:
		BufferLayout() {}
		// Not possible to do it on any other way because the need of two implicit cast
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			: m_Elements(elements) 
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride() {
			
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {

	public:
		VertexBuffer(uint32_t size) {
			glCreateBuffers(1, &m_RendererID);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		};
		VertexBuffer(float* vertices, uint32_t size) {
			glCreateBuffers(1, &m_RendererID);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		};
		~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); };

		void Bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		};
		void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };

		void SetData(const void* data, uint32_t size) {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		};

		const BufferLayout& GetLayout() const { return m_Layout; };
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; };
		const uint32_t& GetBufferID() const { return m_RendererID; };

		static Ref<VertexBuffer> Create(uint32_t size)	{ 
			Ref<VertexBuffer> VBO = CreateRef<VertexBuffer>(size); 
			return VBO; 
		};
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size) { 
			Ref<VertexBuffer> VBO = CreateRef<VertexBuffer>(vertices, size);
			return VBO; 
		};
	private:
		Ref<VertexBuffer> VBO;
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class IndexBuffer {

	public:
		IndexBuffer(uint32_t* indices, uint32_t count)
			: m_Count(count)
		{
			glCreateBuffers(1, &m_RendererID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		};

		~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); };

		void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); };
		void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

		uint32_t GetCount() const { return m_Count; };

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count) {
			Ref<IndexBuffer> IBO = CreateRef<IndexBuffer>(indices, count);
			return IBO;
		};
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}