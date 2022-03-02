#pragma once

#include "pch.h"
#include "Core.h"

namespace Cober {

	class CB_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		void Init();
		void GameLoop();

		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;
	};

	// To be defined in a client
	Application* CreateApplication();
}

