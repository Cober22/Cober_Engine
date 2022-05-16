#pragma once
#include "pch.h"
#include "Cober/Core/Application.h"

namespace Cober {

	class GraphicsContext 
	{
	public:
		GraphicsContext::GraphicsContext(GLFWwindow* windowHandle)
			: m_WindowHandle(windowHandle)
		{
			CB_ASSERT(windowHandle, "Window handle is null");
		}

		virtual void Init() {
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
		};
		virtual void SwapBuffers() { glfwSwapBuffers(m_WindowHandle); };

		static Scope<GraphicsContext> Create(void* window) {
			Scope<GraphicsContext> context = CreateScope<GraphicsContext>(static_cast<GLFWwindow*>(window));
			return context;
		};

	private:
		GLFWwindow* m_WindowHandle;
	};
}