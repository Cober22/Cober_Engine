#include "pch.h"
#include "Texture.h"

#include "Cober/Renderer/Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture2D.h"

namespace Cober {

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None:    CB_ASSERT(false, "RendererAPI::None is currently not supported!");
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
			}

			CB_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
	}
}