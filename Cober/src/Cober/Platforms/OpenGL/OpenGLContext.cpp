#include "pch.h"
#include "OpenGLContext.h"

#include "Cober/Application.h"
#include <GL/GL.h>

namespace Cober {

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		//CB_CORE_ASSERT(windowHandle, "Window handle is null")
	}

	void OpenGLContext::Init()
	{
		SDL_GLContext m_ContextHandle = SDL_GL_CreateContext(m_WindowHandle);
		Application::Get().SetContext(m_ContextHandle);
		SDL_GL_MakeCurrent(m_WindowHandle, m_ContextHandle);

		GLenum error = glewInit();
		//if (error != GLEW_OK)
		//	//CB_CORE_ASSERT(error, "Failed to initialize Glad!");

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void OpenGLContext::SwapBuffers() 
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}
}