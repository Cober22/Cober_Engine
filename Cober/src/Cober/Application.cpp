#include "pch.h"
#include "Application.h"

#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

namespace Cober {
	const static int W_WIDTH = 1024;
	const static int W_HEIGHT = 768;

	Application* Application::s_Instance = nullptr;

	void fatalError(std::string errorString) {

		std::cout << "errorString" << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
	}

	Application::Application() {

		s_Instance = this;
		_context = nullptr;
		_window = nullptr;
		_renderer = nullptr;
		_screenWidth = W_WIDTH;
		_screenHeight = W_HEIGHT;
		_gameState = GameState::PLAY;
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run() {

		Init();
		GameLoop();
	}

	void Application::Init() {

		//// Initialize SDL
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
		_context = SDL_GL_CreateContext(_window);
		if (_context == nullptr)
			fatalError("SDL_GL context could not be created");

		// Set up Glew
		GLenum error = glewInit();
		if (error != GLEW_OK)
			fatalError("Could not initialice glew!");


		ImGui_ImplSDL2_InitForOpenGL(_window, _context);
		ImGui_ImplOpenGL3_Init("#version 460");

		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	
	}

	void Application::GameLoop() {

		while (_gameState != GameState::EXIT) 
		{
			glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			ProcessInputs();
			DrawGame();
		}
	}

	void Application::ProcessInputs() {

		SDL_Event event;

		// Dispatcher events
		while (SDL_PollEvent(&event)) {

			ImGui_ImplSDL2_ProcessEvent(&event);

			switch (event.type) {
				case SDL_QUIT:
					_gameState = GameState::EXIT;
				break;
				case SDL_MOUSEMOTION:
					std::cout << event.motion.x << " " << event.motion.y << std::endl;
					break;
			}
		}
	}

	void Application::DrawGame() {



		SDL_GL_SwapWindow(_window);
	}
}
