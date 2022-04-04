#include "pch.h"
#include "WindowsWindow.h"

#include "Platforms/OpenGL/OpenGLContext.h"

namespace Cober {

	static bool s_SDLInitialized = false;

	static void SDLErrorCallback(int error, const char* description) 
	{
		//CB_CORE_ERROR("SDL Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) 
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		//CB_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_SDLInitialized) 
		{
			// TODO: SDL_Quit(); on system shutdhown
			int success = SDL_Init(SDL_INIT_EVERYTHING);
			//CB_CORE_ASSERT(success, "Could not initialize SDL!");
			//sdlSetErrorCallback(SDLErrorCallback);
			s_SDLInitialized = true;
		}


		uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;// | SDL_WINDOW_MOUSE_CAPTURE;

		// Open an SDL window
		m_Window = SDL_CreateWindow(m_Data.Title.c_str(),		// Window title
									SDL_WINDOWPOS_CENTERED,		// posX on screen
									SDL_WINDOWPOS_CENTERED,		// posY on screen
									(int)m_Data.Width,			// width of the window
									(int)m_Data.Height,			// height of the window
									flags);						// flags

		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();

		SetVSync(true);

		// Set SDL callbacks
		/*
		SDL_SetWindowSize(m_Window, [](SDL_Window* window, int width, int height)
		{
			WindowData& data = *(WindowData*)SDL_GetWindowData(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		
		SDL_CLOSE(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		SDL_KeyboardEvent(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});*/
	}

	void Cober::WindowsWindow::Shutdown()
	{
		SDL_DestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		m_Context->SwapBuffers();
	}
	
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);

		m_Data.VSync = enabled;
	}
	
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}
