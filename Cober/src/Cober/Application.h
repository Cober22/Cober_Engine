#pragma once

#include "pch.h"
#include "Core.h"
#include "Layers/LayerStack.h"

#include "Window.h"

#include "ImGui/ImGuiLayer.h"
#include "Cober/Renderer/Buffer.h"
#include "Cober/Renderer/Shader.h"

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
		inline Window& GetWindow() { return *_window; }

		inline SDL_GLContext GetContext() { return _context; }
		inline void SetContext(SDL_GLContext context) { _context = context; }
	private:
		void ProcessInputs();
		
		Window* _window;
		SDL_GLContext _context;
		GameState _gameState;

		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;//, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};

	// To be defined in a client
	Application* CreateApplication();
}