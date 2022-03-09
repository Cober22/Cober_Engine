#include "pch.h"
#include "ImGui/ImGuiLayer.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"

#include "Application.h"

namespace Cober {

	Application* Application::s_Instance = nullptr;

	Application::Application() {

		s_Instance = this;
		_window = Window::Create();

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
			glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			_window->OnUpdate();

			ProcessInputs();
		}
	}

	void Application::ProcessInputs() {

		SDL_Event event;

		// Dispatcher events
		while (SDL_PollEvent(&event)) {
			
			ImGui_ImplSDL2_ProcessEvent(&event);

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
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
