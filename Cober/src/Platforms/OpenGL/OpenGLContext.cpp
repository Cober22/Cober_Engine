#include "pch.h"
#include "OpenGLContext.h"

#include "Cober/Application.h"

#include <Glew/Glew.h>
#include <GL/GL.h>
#include <SDL/SDL.h>

namespace Cober {

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CB_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		SDL_GLContext m_ContextHandle = SDL_GL_CreateContext(m_WindowHandle);
		Application::Get().SetContext(m_ContextHandle);
		SDL_GL_MakeCurrent(m_WindowHandle, m_ContextHandle);

		GLenum error = glewInit();
		if (error != GLEW_OK)
			CB_ASSERT(false, ("{0}, Failed to initialize Glad!", (const char*)error));

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void OpenGLContext::SwapBuffers() 
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}
}