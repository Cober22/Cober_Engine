#include "pch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"


namespace Cober {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    SDL_LogInfo(0, "RendererAPI::None is currently not supported!");	//CB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLVertexArray();
		}

		SDL_LogInfo(0, "Unknown RendererAPI!");	//CB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}