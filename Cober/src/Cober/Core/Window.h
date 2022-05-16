#pragma once
#include "pch.h"
#include "Core.h"

#include "Cober/Events/EventManager.h"
#include "Cober/Events/ApplicationEvents.h"
#include "Cober/Events/KeyEvents.h"
#include "Cober/Events/MouseEvents.h"
#include "Cober/Events/Input.h"

#include "Cober/Renderer/GraphicsContext.h"
#include "Cober/Renderer/Renderer.h"

namespace Cober {

	struct WindowProps {
		
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool FullScreen;

		WindowProps(const std::string& title = "Cober Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			bool fullscreen = false)
			: Title(title), Width(width), Height(height), FullScreen(fullscreen)
		{
		}
	};

	// Interface representing a desktop system based Window
	static uint8_t s_GLFWWindowCount = 0;
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window::Window(const WindowProps& props) {
			CB_PROFILE_FUNCTION();
			Init(props);
		};
		~Window() { CB_PROFILE_FUNCTION();	Shutdown();	};

		void Init(const WindowProps& props) {
			CB_PROFILE_FUNCTION();

			m_Data.Title = props.Title;
			m_Data.Width = props.Width;
			m_Data.Height = props.Height;

			// Init GLFW
			if (s_GLFWWindowCount == 0)
			{
				CB_PROFILE_SCOPE("glfwInit");
				int success = glfwInit();
				CB_ASSERT(success, "Could not initialize GLFW!");
			}

			// Init WINDOW
			m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;

			// Init Window CONTEXT
			m_Context = GraphicsContext::Create(m_Window);
			m_Context->Init();

			glfwSetWindowUserPointer(m_Window, &m_Data);
			SetVSync(true);

			Callbacks();
		};
		void Callbacks() {
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					data.Width = width;
					data.Height = height;

					WindowResizeEvent event(width, height);
					data.EventCallback(event);
				});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					WindowCloseEvent event;
					data.EventCallback(event);
				});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(static_cast<KeyCode>(key), 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(static_cast<KeyCode>(key));
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(static_cast<KeyCode>(key), 1);
						data.EventCallback(event);
						break;
					}
					}
				});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					KeyTypedEvent event(static_cast<KeyCode>(keycode));
					data.EventCallback(event);
				});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(static_cast<MouseCode>(button));
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
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
				});
		};
		void Shutdown() {
			CB_PROFILE_FUNCTION();

			glfwDestroyWindow(m_Window);
			--s_GLFWWindowCount;

			if (s_GLFWWindowCount == 0)
				glfwTerminate();
		};

		void OnUpdate() { glfwPollEvents(); m_Context->SwapBuffers(); };

		uint32_t GetWidth() const	{ return m_Data.Width; };
		uint32_t GetHeight() const	{ return m_Data.Height; };
		void SetWidth(float width)	{ m_Data.Width = width; };
		void SetHeight(float height){ m_Data.Height = height; };

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; };
		void SetVSync(bool enabled) {
			CB_PROFILE_FUNCTION();

			if (enabled)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);

			m_Data.VSync = enabled;
		};
		bool IsVSync() const { return m_Data.VSync; };

		GLFWwindow* GetNativeWindow() const { return m_Window; };

		static Scope<Window> Create(const WindowProps& props = WindowProps())
		{
#ifdef CB_PLATFORM_WINDOWS
			return CreateScope<Window>(props);
#else
			CB_ASSERT(false, "Unknown platform!");
			return nullptr;
#endif
		};
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}