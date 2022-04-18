#include "pch.h"
#include "GraphicsContext.h"

#include "Cober/Renderer/Renderer.h"
#include "Platforms/OpenGL/OpenGLContext.h"

namespace Cober {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CB_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		CB_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}