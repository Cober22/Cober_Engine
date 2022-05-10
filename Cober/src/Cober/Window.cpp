#include "checkML.h"
#include "pch.h"
#include "Window.h"

#ifdef CB_PLATFORM_WINDOWS
#include "Platforms/Windows/WindowsWindow.h"
#endif

namespace Cober
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef CB_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		CB_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}