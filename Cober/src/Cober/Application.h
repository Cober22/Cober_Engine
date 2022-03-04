#pragma once

#include "pch.h"
#include "Core.h"
#include "LayerStack.h"

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

	private:
		void Init();
		void GameLoop();
		void ProcessInputs();
		void DrawGame();

		SDL_Window* _window;
		SDL_Renderer* _renderer;
		GameState _gameState;
		int _screenWidth;
		int _screenHeight;
		LayerStack m_LayerStack;
	};

	// To be defined in a client
	Application* CreateApplication();
}