#pragma once

#include "pch.h"
#include "Core.h"

namespace Cober {

	enum class GameState { PLAY, EXIT };

	class CB_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		SDL_Window* _window;
		void Init();
		void GameLoop();
		void ProcessInputs();
		void DrawGame();

		SDL_Renderer* _renderer;
		GameState _gameState;
		int _screenWidth;
		int _screenHeight;
	};

	// To be defined in a client
	Application* CreateApplication();
}