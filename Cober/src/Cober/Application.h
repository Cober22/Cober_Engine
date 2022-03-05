#pragma once

#include "pch.h"
#include "Core.h"
#include "Layers/LayerStack.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline SDL_Window* GetWindow() { return _window; }
		inline SDL_Renderer* GetRenderer() { return _renderer; }
		inline SDL_GLContext GetContext() { return _context; }
		inline int GetWindowWidth() { return _screenHeight; }
		inline int GetWindowHeight() { return _screenWidth; }
	private:
		void Init();
		void GameLoop();
		void ProcessInputs();
		void DrawGame();

		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _context;
		GameState _gameState;
		int _screenWidth;
		int _screenHeight;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in a client
	Application* CreateApplication();
}