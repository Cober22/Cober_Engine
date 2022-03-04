#include "pch.h"
#include "Application.h"

namespace Cober {
	const int W_WIDTH = 1024;
	const int W_HEIGHT = 768;
			
	void fatalError(std::string errorString) {
		std::cout << "errorString" << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
	}

	Application::Application() {

		_window = nullptr;
		_renderer = nullptr;
		_screenWidth = W_WIDTH;
		_screenHeight = W_HEIGHT;
		_gameState = GameState::PLAY;
	}

	Application::~Application() {

	}

	void Application::Run() {

		Init();
		GameLoop();
	}

	void Application::Init() {

		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Open an SDL window
		_window = SDL_CreateWindow("Graphics Engine",		// Window title
									SDL_WINDOWPOS_CENTERED,	// posX on screen
									SDL_WINDOWPOS_CENTERED,	// posY on screen
									_screenWidth,			// width of the window
									_screenHeight,			// height of the window
									SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);		// flags
		if (_window == nullptr)
			fatalError("SDL Window could not be created!");
				
		// OpenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(_window);
		if (glContext == nullptr)
			fatalError("SDL_GL context could not be created");

		// Set up Glew
		GLenum error = glewInit();
		if (error != GLEW_OK)
			fatalError("Could not initialice glew!");

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		
		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
	}

	void Application::GameLoop() {

		while (_gameState != GameState::EXIT) 
		{
			ProcessInputs();
			DrawGame();
		}
	}

	void Application::ProcessInputs() {

		SDL_Event event;

		// Dispatcher events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					_gameState = GameState::EXIT;
					break;
				case SDL_MOUSEMOTION:
					std::cout << event.motion.x << " " << event.motion.y << std::endl;
					break;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					_screenWidth = event.window.data1;
					_screenHeight = event.window.data2;
					SDL_RenderPresent(_renderer);
					break;
			}
		}
	}

	void Application::DrawGame() {

		glClear(GL_COLOR_BUFFER_BIT);


		SDL_GL_SwapWindow(_window);
	}
}
