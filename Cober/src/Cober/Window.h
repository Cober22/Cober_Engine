#pragma once

#include "pch.h"

#include "Cober/Core.h"
#include "Cober/Events/EventManager.h"

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
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetWidth(float width) = 0;
		virtual void SetHeight(float height) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual GLFWwindow* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	public:
		//static float s_HighDPIScaleFactor;
	};
}