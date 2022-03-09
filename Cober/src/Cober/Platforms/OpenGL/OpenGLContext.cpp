#include "pch.h"
#include "OpenGLContext.h"

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#include <GL/GL.h>

namespace Cober {

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		//CB_CORE_ASSERT(windowHandle, "Window handle is null")
	}

	void OpenGLContext::Init()
	{
		//SDL_GLContext _context = SDL_GL_CreateContext(m_WindowHandle);
		//SDL_GL_MakeCurrent(m_WindowHandle, _context);

		//GLenum error = glewInit();
		////if (error != GLEW_OK)
		//	//CB_CORE_ASSERT(error, "Failed to initialize Glad!");



		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void OpenGLContext::SwapBuffers() 
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}
}