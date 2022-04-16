#pragma once

#include "pch.h"
#include "Core.h"
#include "Layers/LayerStack.h"

#include "Window.h"

#include "ImGui/ImGuiLayer.h"
#include "Cober/Renderer/Shader.h"
#include "Cober/Renderer/Buffer.h"
#include "Cober/Renderer/VertexArray.h"

namespace Cober {

	enum class GameState { PLAY, EXIT };

	class Application
	{
	public:
		Application(const std::string& name = "");
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();
		void Run();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *_window; }
	private:
		void ProcessInputs();
		
		Window* _window;
		GameState _gameState;

		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		float timeInSeconds = 0.0f;
		int frames = 0;
		bool w_Minimized = false;
		bool w_Fullscreen = false;
		const int FPS_LIMIT = 10;


	private:
		static Application* s_Instance;
	};

	// To be defined in a client
	Application* CreateApplication();
}