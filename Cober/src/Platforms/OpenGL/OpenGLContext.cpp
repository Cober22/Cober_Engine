#include "pch.h"
#include "OpenGLContext.h"

#include "Cober/Application.h"

//#include <GL/GL.h>

namespace Cober {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CB_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		GLenum error = glewInit();
		if (error != GLEW_OK)
			CB_ASSERT(false, ("{0}, Failed to initialize Glew!", (const char*)error));

		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//
		//// Check for OpenGL Version
		//#ifdef CB_ENABLE_ASSERTS
		//		int versionMajor;
		//		int versionMinor;
		//		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		//		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		//
		//		CB_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "CoberEngine requires at least OpenGL version 4.6!");
		//#endif
	}

	void OpenGLContext::SwapBuffers() 
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}