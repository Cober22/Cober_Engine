#include "pch.h"
#include "Application.h"
<<<<<<< Updated upstream
=======

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#include "ImGui/ImGuiLayer.h"
>>>>>>> Stashed changes

#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

namespace Cober {

	Application* Application::s_Instance = nullptr;
	
	Application::Application() {

		s_Instance = this;
<<<<<<< Updated upstream
		_context = nullptr;
		_window = nullptr;
		_renderer = nullptr;
		_screenWidth = 1024;
		_screenHeight = 768;
=======

		_window = std::unique_ptr<Window>(Window::Create());
		
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
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
=======
		SDL_Window* window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		SDL_GLContext _context = SDL_GL_CreateContext(window);

>>>>>>> Stashed changes
		GLenum error = glewInit();
		//if (error != GLEW_OK)
			//CB_CORE_ASSERT(error, "Failed to initialize Gl3W!");

		ImGui_ImplSDL2_InitForOpenGL(window, _context);
		ImGui_ImplOpenGL3_Init("#version 460");

<<<<<<< Updated upstream
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	
=======

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
>>>>>>> Stashed changes
	}

	void Application::GameLoop() {

		while (_gameState != GameState::EXIT) 
		{
			glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			ProcessInputs();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

<<<<<<< Updated upstream
			ProcessInputs();
			DrawGame();
=======
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			_window->OnUpdate();
>>>>>>> Stashed changes
		}
	}

	void Application::ProcessInputs() {

		SDL_Event event;

		// Dispatcher events
		while (SDL_PollEvent(&event)) {
<<<<<<< Updated upstream

			// Process Layer Events
			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
=======
			
			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
>>>>>>> Stashed changes
				(*--it)->OnEvent(event);

			switch (event.type) {
				case SDL_QUIT:
					_gameState = GameState::EXIT;
				break;
				case SDL_MOUSEMOTION:
					//std::cout << event.motion.x << " " << event.motion.y << std::endl;
					break;
			}
		}
	}
}
