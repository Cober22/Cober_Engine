#pragma once

#include "Core.h"
#include "Window.h"
#include "Layers/LayerStack.h"
#include "Events/EventManager.h"
#include "Events/ApplicationEvents.h"

#include "ImGui/ImGuiLayer.h"

namespace Cober {

	enum class GameState { PLAY, EXIT };

	class Application
	{
	public:
		Application(const std::string& name = "");
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& event) {

			CB_PROFILE_FUNCTION();

			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<WindowCloseEvent>(CB_BIND_EVENT(Application::OnWindowClose));
			dispatcher.Dispatch<WindowResizeEvent>(CB_BIND_EVENT(Application::OnWindowResize));

			for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
			{
				if (event.Handled)
					break;
				(*it)->OnEvent(event);
			}
		}

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void ProcessInputs();

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *_window; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> _window;
		GameState _gameState;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		float timeInSeconds = 0.0f;
		int frames = 0;
		bool m_CursorMode = false;
		bool w_Minimized = false;
		bool w_Maximized = false;
		bool w_Fullscreen = false;
		const int FPS_LIMIT = 10;
	private:
		static Application* s_Instance;
		//friend int ::main(int argc, char** argv);
	};

	// To be defined in a client
	Application* CreateApplication();
}