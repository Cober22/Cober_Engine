#include "pch.h"

#include "Application.h"
#include "Cober/Timestep.h"
#include "Cober/Renderer/Renderer.h"

namespace Cober {

	Application* Application::s_Instance = nullptr;
	const float W_WIDTH = 1280.0f;
	const float W_HEIGHT = 720.0f;

	Application::Application() {

		s_Instance = this;
		WindowProps windowProps = WindowProps("Cober Engine", W_WIDTH, W_HEIGHT);
		_window = Window::Create(windowProps);

		Renderer::Init();

		_gameState = GameState::PLAY;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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

		while (_gameState != GameState::EXIT)
		{
			ProcessInputs();

			float time = (float)(SDL_GetTicks() / 1000.0f);
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			_window->OnUpdate();
		}
	}

	void Application::ProcessInputs() {

		SDL_Event event;
		SDL_Window* window = _window->GetNativeWindow();
		
		// Dispatcher events
		while (SDL_PollEvent(&event)) {
			
			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
				(*--it)->OnEvent(event);

			switch (event.type) {
				case SDL_QUIT:
					_gameState = GameState::EXIT;	break;
				case SDL_KEYDOWN:
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						_gameState = GameState::EXIT;
					if (event.key.keysym.scancode == SDL_SCANCODE_F) {
						w_fullscreen = w_fullscreen == true ? false : true;

						if (w_fullscreen) {
							//SDL_SetWindowSize(window, W_WIDTH, W_HEIGHT);		// TEST
							SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
						}
						else {
							SDL_SetWindowFullscreen(window, 0);
							//SDL_SetWindowSize(window, 800.0f, 600.0f);	// TEST
						}
					}
					break;
				//case SDL_WINDOWEVENT_SIZE_CHANGED:
			}
		}
	}
}
