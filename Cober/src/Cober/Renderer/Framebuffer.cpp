#include "checkML.h"
#include "pch.h"
#include "Framebuffer.h"

#include "Cober/Renderer/Renderer.h"

#include "Platforms/OpenGL/OpenGLFramebuffer.h"

namespace Cober {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CB_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		CB_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}