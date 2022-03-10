#include "pch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLBuffer.h"

namespace Cober {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    SDL_LogInfo(0, "RendererAPI::None is currently not supported!");	//CB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		}

		SDL_LogInfo(0, "Unknown RendererAPI!");	//CB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    SDL_LogInfo(0, "RendererAPI::None is currently not supported!");	//CB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLIndexBuffer(indices, size);
		}

		SDL_LogInfo(0, "Unknown RendererAPI!");	//CB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}