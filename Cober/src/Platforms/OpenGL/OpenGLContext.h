#pragma once

#include "Cober/Renderer/GraphicsContext.h"

struct SDL_Window;

namespace Cober {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(SDL_Window* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		SDL_Window* m_WindowHandle;
	};
}
