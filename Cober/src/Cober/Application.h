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

		void OnEvent(Event& e)
		{
			CB_PROFILE_FUNCTION();

			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowCloseEvent>(CB_BIND_EVENT(Application::OnWindowClose));
			dispatcher.Dispatch<WindowResizeEvent>(CB_BIND_EVENT(Application::OnWindowResize));

			for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
			{
				if (e.Handled)
					break;
				(*it)->OnEvent(e);
			}
		};

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

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
		bool w_Minimized = false;
		bool w_Fullscreen = false;
		const int FPS_LIMIT = 10;
	private:
		static Application* s_Instance;
		//friend int ::main(int argc, char** argv);
	};

	// To be defined in a client
	Application* CreateApplication();
}