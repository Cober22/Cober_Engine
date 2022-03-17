#include "pch.h"
#include "Shader.h"
#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLShader.h"

namespace Cober {

	Shader* Shader::Create(const char* vertexPath, const char* fragmentPath) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CB_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexPath, fragmentPath);
		}

		CB_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
