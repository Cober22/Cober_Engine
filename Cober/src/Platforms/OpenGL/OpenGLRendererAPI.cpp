#include "pch.h"
#include "OpenGLRendererAPI.h"

#include <Glew/Glew.h>
#include <GL/GL.h>
#include <SDL/SDL.h>



namespace Cober {

	void OpenGLRendererAPI::Init() 
	{
		glEnable(GL_DEPTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_MULTISAMPLE);
		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	// In the future will receibe flags as arguments
	void OpenGLRendererAPI::Clear()	
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// In the future will receibe primitive types for rendering 
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}