#include "pch.h"
#include "Application.h"

namespace Cober {
	const int W_WIDTH = 1024;
	const int W_HEIGHT = 768;

	Application::Application()
	{
		_window = nullptr;
		_screenWidth = W_WIDTH;
		_screenHeight = W_HEIGHT;
	}

	Application::~Application()
	{
	}

	void Application::Run() {
		Init();
		GameLoop();
	}

	void Application::Init() {
		SDL_Init(SDL_INIT_EVERYTHING);

		_window = SDL_CreateWindow("Graphics Engine",		// Window title
									SDL_WINDOWPOS_CENTERED,	// posX on screen
									SDL_WINDOWPOS_CENTERED,	// posY on screen
									_screenWidth,			// width of the window
									_screenHeight,			// height of the window
									SDL_WINDOW_OPENGL);		// flags
	}

	void Application::GameLoop() {
		while (true);
	}
}
