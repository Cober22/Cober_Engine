#include "pch.h"
#include "OpenGLVertexArray.h"

#include <Glew/Glew.h>

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

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() 
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		CB_LogInfo(LOG_RENDER, ("Size: {0} - VertexBuffer has no layout!", (const char*)vertexBuffer->GetLayout().GetElements().size()));
		
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
