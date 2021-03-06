#pragma once

#include "pch.h"
#include "Cober/Renderer/Buffer.h"

namespace Cober {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Cober::ShaderDataType::Float:		return GL_FLOAT;
		case Cober::ShaderDataType::Float2:		return GL_FLOAT;
		case Cober::ShaderDataType::Float3:		return GL_FLOAT;
		case Cober::ShaderDataType::Float4:		return GL_FLOAT;
		case Cober::ShaderDataType::Mat3:		return GL_FLOAT;
		case Cober::ShaderDataType::Mat4:		return GL_FLOAT;
		case Cober::ShaderDataType::Int:		return GL_INT;
		case Cober::ShaderDataType::Int2:		return GL_INT;
		case Cober::ShaderDataType::Int3:		return GL_INT;
		case Cober::ShaderDataType::Int4:		return GL_INT;
		case Cober::ShaderDataType::Bool:		return GL_BOOL;
		}

		CB_LogInfo(LOG_RENDER, "Uknown Shader Data Type!");
		return 0;
	}

	class VertexArray
	{
	public:
		VertexArray() { glCreateVertexArrays(1, &m_RendererID); };
		~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); };

		void Bind() const { glBindVertexArray(m_RendererID); };
		void Unbind() const { glBindVertexArray(0); };

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
			CB_LogInfo(LOG_RENDER, ("Size: {0} - VertexBuffer has no layout!", (const char*)vertexBuffer->GetLayout().GetElements().size()));

			glBindVertexArray(m_RendererID);
			vertexBuffer->Bind();

			const auto& layout = vertexBuffer->GetLayout();
			for (const auto& element : layout)
			{
				switch (element.Type)
				{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset);
					index++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset);
					index++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(index);
						glVertexAttribPointer(index,
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(index, 1);
						index++;
					}
					break;
				}
				default:
					CB_ASSERT(false, "Unknown ShaderDataType!");
				}
			}

			m_VertexBuffers.push_back(vertexBuffer);
		};
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
			CB_PROFILE_FUNCTION();

			glBindVertexArray(m_RendererID);
			indexBuffer->Bind();

			m_IndexBuffer = indexBuffer;
		};

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; };
		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };
		const uint32_t GetRendererID() const { return m_RendererID; };

		static Ref<VertexArray> Create() { return CreateRef<VertexArray>(); };

	private:
		uint32_t m_RendererID;
		uint32_t index = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}