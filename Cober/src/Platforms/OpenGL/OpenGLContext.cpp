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

		std::cout << "OpenGL Info:" << std::endl;
		std::cout << "Vendor:\t" << (const char*)glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer:\t" << (const char*)glGetString(GL_RENDERER) << std::endl;
		std::cout << "Version:\t" << (const char*)glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL:\t\t" << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		
		// Check for OpenGL Version
		#ifdef CB_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		
				CB_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "CoberEngine requires at least OpenGL version 4.6!");
		#endif
	}

	void OpenGLContext::SwapBuffers() 
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}