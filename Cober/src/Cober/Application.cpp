#include "pch.h"

#include "Application.h"

namespace Cober {

	Application* Application::s_Instance = nullptr;

	Application::Application() {

		s_Instance = this;
		_window = Window::Create();

		_gameState = GameState::PLAY;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// FIRST TRIANGLE!
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 3] = {
			-0.33f, -0.5f, 1.0f,
			 0.33f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// SHADER (PROVISIONAL)
		// Vertex 
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;

			void main() 
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main() 
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);	
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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

			m_Shader->Bind();
			// DRAW TRIANGLE!
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount() , GL_UNSIGNED_INT, nullptr);

			ProcessInputs();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			_window->OnUpdate();
		}
	}

	void Application::ProcessInputs() {

		SDL_Event event;
		
		// Dispatcher events
		while (SDL_PollEvent(&event)) {
			
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
