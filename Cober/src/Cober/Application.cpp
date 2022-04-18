#include "pch.h"

#include "Application.h"

//#include "Cober/Log.h"
#include "Cober/Events/Input.h"
#include "Cober/Renderer/Renderer.h"
#include "Cober/Timestep.h"

namespace Cober {

	Application* Application::s_Instance = nullptr;
	const float W_WIDTH = 1280.0f;
	const float W_HEIGHT = 720.0f;

	Application::Application(const std::string& name) {

		CB_PROFILE_FUNCTION();

		s_Instance = this;
		WindowProps windowProps = WindowProps("Cober Engine", W_WIDTH, W_HEIGHT);
		_window = Window::Create(WindowProps(name));
		_window->SetEventCallback(CB_BIND_EVENT(Application::OnEvent));

		Renderer::Init();

		_gameState = GameState::PLAY;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {

		CB_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {

		CB_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {

		CB_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	//void Application::OnEvent(Event& event) {

	//	CB_PROFILE_FUNCTION();

	//	EventDispatcher dispatcher(event);
	//	dispatcher.Dispatch<WindowCloseEvent>(CB_BIND_EVENT(Application::OnWindowClose));
	//	dispatcher.Dispatch<WindowResizeEvent>(CB_BIND_EVENT(Application::OnWindowResize));

	//	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	//	{
	//		if (event.Handled)
	//			break;
	//		(*it)->OnEvent(event);
	//	}
	//}

	void Application::Run() {

		while (_gameState != GameState::EXIT)
		{
			CB_PROFILE_SCOPE("RunLoop");

			float time = (float)(glfwGetTime());
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

			if (!w_Minimized)
			{
				{
					CB_PROFILE_SCOPE("Render Prep");
					//RenderCommand::SetClearColor({ 0.02f, 0.008f, 0.05f, 1.0f });	// DARK BLUE
					//RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
					RenderCommand::SetClearColor({ 1.0f, 0.6f, 0.3f, 1.0f });	// ORANGE
					//RenderCommand::SetClearColor({ 0.8f, 0.35f, 0.35f, 1.0f });
					RenderCommand::Clear();
				}
				{
					CB_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
				{
					CB_PROFILE_SCOPE("LayerStack OnImGuiRenderer");

					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			
			_window->OnUpdate();
		}
	}

	void Application::Close() {
		_gameState = GameState::EXIT;
	}

	//void Application::OnEvent(Event& e)
	//{
	//	CB_PROFILE_FUNCTION();

	//	EventDispatcher dispatcher(e);
	//	dispatcher.Dispatch<WindowCloseEvent>(CB_BIND_EVENT(Application::OnWindowClose));
	//	dispatcher.Dispatch<WindowResizeEvent>(CB_BIND_EVENT(Application::OnWindowResize));

	//	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	//	{
	//		if (e.Handled)
	//			break;
	//		(*it)->OnEvent(e);
	//	}
	//}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		_gameState = GameState::EXIT;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		CB_PROFILE_FUNCTION();

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			w_Minimized = true;
			return false;
		}

		w_Minimized = false;
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

		return false;
	}
}
