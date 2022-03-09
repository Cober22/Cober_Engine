#pragma once

#include "pch.h"
#include "Core.h"
#include "Layers/LayerStack.h"
#include "Window.h"

namespace Cober {

	enum class GameState { PLAY, EXIT };

	class CB_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *_window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		void Init();
		void GameLoop();
		void ProcessInputs();
<<<<<<< Updated upstream
		void DrawGame();

		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _context;
=======

		std::unique_ptr<Window> _window;
>>>>>>> Stashed changes
		GameState _gameState;
		int _screenWidth;
		int _screenHeight;
		LayerStack m_LayerStack;
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
	private:
		static Application* s_Instance;
	};

	// To be defined in a client
	Application* CreateApplication();
}