#include "pch.h"

#include "Application.h"
#include "Cober/Renderer/Renderer.h"

//Quitar cuando sea abstraido
#include <SDL/SDL.h>

namespace Cober {

	Application* Application::s_Instance = nullptr;

	Application::Application() {

		s_Instance = this;
		_window = Window::Create();

		_gameState = GameState::PLAY;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// FIRST TRIANGLE!
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.33f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.33f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,   0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"  }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// SHADER (PROVISIONAL)
		// Vertex 
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() 
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() 
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);	
				color = v_Color;
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
			RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();
			
			//m_VertexArray->Bind();
			//glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount() , GL_UNSIGNED_INT, nullptr);

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
