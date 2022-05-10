#include "checkML.h"
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

	void Application::Close() {
		_gameState = GameState::EXIT;
	}

	void Application::Run() {

		while (_gameState != GameState::EXIT)
		{
			CB_PROFILE_SCOPE("RunLoop");

			float time = (float)(glfwGetTime());
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			timeInSeconds += timestep;
			frames++;

			// Frame rate
			if (timeInSeconds >= 1.0f) {
				timestep.SetFrames(frames);
				frames = 0;
				timeInSeconds = 0;
			}
			//if (1000/ FPS_LIMIT > m_LastFrameTime)
				//SDL_Delay(1000 / FPS_LIMIT);

			if (!w_Minimized)
			{
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
					m_ImGuiLayer->End();
				}
				ProcessInputs();
			}
			
			_window->OnUpdate();
		}
	}

	void Application::ProcessInputs() {
		
		if (Input::IsKeyPressedOne(KEY_M)) {
			m_CursorMode = m_CursorMode == true ? false : true;
			if (m_CursorMode)
				glfwSetInputMode(_window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(_window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		if (Input::IsKeyPressedOne(KEY_ESCAPE))
			_gameState = GameState::EXIT;
		if (Input::IsKeyPressedOne(KEY_F)) {
			w_Maximized = w_Maximized == true ? false : true;
			if (w_Maximized)
				glfwMaximizeWindow(_window->GetNativeWindow());
			else
				glfwRestoreWindow(_window->GetNativeWindow());
		}
	}

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
