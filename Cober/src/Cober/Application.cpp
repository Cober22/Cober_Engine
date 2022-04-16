#include "pch.h"

#include "Application.h"
#include "Cober/Timestep.h"
#include "Cober/Renderer/Renderer.h"

namespace Cober {

	Application* Application::s_Instance = nullptr;
	const float W_WIDTH = 1280.0f;
	const float W_HEIGHT = 720.0f;

	Application::Application(const std::string& name) {

		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "4");

		s_Instance = this;
		WindowProps windowProps = WindowProps("Cober Engine", W_WIDTH, W_HEIGHT);
		_window = Window::Create(WindowProps(name));

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

	void Application::Close() {
		_gameState = GameState::EXIT;
	}

	void Application::Run() {

		while (_gameState != GameState::EXIT)
		{
			float time = (float)(SDL_GetTicks() / 1000.0f);
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			timeInSeconds += timestep;
			frames++;

			// Frame rate limit
			if (timeInSeconds >= 1.0f) {
				//std::cout << frames << std::endl;
				frames = 0;
				timeInSeconds = 0;
			}
			//if (1000/ FPS_LIMIT > m_LastFrameTime)
				//SDL_Delay(1000 / FPS_LIMIT);

			if (!w_Minimized) {
				ProcessInputs();
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			_window->OnUpdate();
		}
	}

	void Application::ProcessInputs() {

		SDL_Event event; 
		GLFWwindow* window = _window->GetNativeWindow();
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
					//if (event.key.keysym.scancode == SDL_SCANCODE_F) {
					//	w_Fullscreen = w_Fullscreen == true ? false : true;
					//
					//	if (w_Fullscreen)
					//		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
					//	else 
					//		SDL_SetWindowFullscreen(window, 0);
					//}
					//if (event.key.keysym.scancode == SDL_SCANCODE_M) {
					//	if (SDL_GetRelativeMouseMode() == SDL_TRUE)
					//		SDL_SetRelativeMouseMode(SDL_FALSE);
					//	else
					//		SDL_SetRelativeMouseMode(SDL_TRUE);
					//}
					break;
				case SDL_WINDOWEVENT:
					if (_window->GetWidth() == 0 || _window->GetHeight() == 0)
						w_Minimized = true;
					else {
						int* width = nullptr, *height = nullptr;

						glfwGetWindowSize(window, width, height);
						_window->SetWidth((float&)width);
						_window->SetHeight((float&)height);
						w_Minimized = false;
						Renderer::OnWindowResize((float&)width, (float&)height);
					}
			}
		}
	}
}
