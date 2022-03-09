#pragma once

#include "pch.h"

#include "Cober/Core.h"

namespace Cober {

	struct WindowProps {
		
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Cober Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) 
		{
		}
	};

	// Interface representing a desktiop system based Window
	class Window
	{
	public:
		//using EventCallbackFn = std::function<void(SDL_Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		//virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual void* GetContext() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}