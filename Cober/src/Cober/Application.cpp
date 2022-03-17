#include "pch.h"

#include "Application.h"
#include "Cober/Timestep.h"
#include "Cober/Renderer/Renderer.h"

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

		//SDL_Event event;
		//
		//// Dispatcher events
		//while (SDL_PollEvent(&event)) {
		//	
		//	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		//		(*--it)->OnEvent(event);

		//	switch (event.type) {
		//		case SDL_QUIT:
		//			_gameState = GameState::EXIT;
		//		break;
		//	}
		//}
	}
}
