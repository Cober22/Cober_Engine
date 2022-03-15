#include "pch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace Cober {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CB_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}

		CB_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}