#pragma once

#include "pch.h"
#include "Core.h"
#include "Layers/LayerStack.h"

#include "ImGui/ImGuiLayer.h"

namespace Cober {

	enum class GameState { PLAY, EXIT };

	class Application
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

		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		unsigned int _screenHeight;
		unsigned int _screenWidth;
	private:
		static Application* s_Instance;
	};

	// To be defined in a client
	Application* CreateApplication();
}