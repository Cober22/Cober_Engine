#include "pch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

#include <SDL/SDL.h>

namespace Cober {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SDL_LogInfo(0, "RendererAPI::None is currently not supported!");	//CB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}

		SDL_LogInfo(0, "Unknown RendererAPI!");	//CB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}