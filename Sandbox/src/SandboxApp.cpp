#include <Engine.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Cober::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Cober::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Cober::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Cober::VertexBuffer::Create(vertices, sizeof(vertices)));
		Cober::BufferLayout layout = {
			{ Cober::ShaderDataType::Float3, "a_Position" },
			{ Cober::ShaderDataType::Float4, "a_Color"  }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Cober::IndexBuffer> indexBuffer;
		indexBuffer.reset(Cober::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader.reset(Cober::Shader::Create("Shaders/shader.vs", "Shaders/shader.fs"));
	}

	void OnUpdate(Cober::Timestep ts) override
	{
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
		Cober::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		//m_Camera.SetRotation(45.0f);

		Cober::Renderer::BeginScene(m_Camera);
		
		Cober::Renderer::Submit(m_Shader, m_VertexArray);

		Cober::Renderer::EndScene();


		SDL_Event event;

		// Dispatcher events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					std::cout << "SDIJAOISDJIOAJSIDIO" << std::endl;
					case SDLK_LEFT:
						std::cout << "LEFT" << std::endl;
						m_CameraPosition.x += m_CameraSpeed * ts;
						break;
					case SDLK_RIGHT:
						std::cout << "RIGHT" << std::endl;
						m_CameraPosition.x -= m_CameraSpeed * ts;
						break;
					case SDLK_DOWN:
						std::cout << "DOWN" << std::endl;
						m_CameraPosition.y += m_CameraSpeed * ts;
						break;
					case SDLK_UP:
						std::cout << "UP" << std::endl;
						m_CameraPosition.y -= m_CameraSpeed * ts;
						break;
				}
			}
		}
	}

	virtual void OnImGuiRender() override 
	{
	}

	void OnEvent(SDL_Event& event) override
	{
	}

private:
	std::shared_ptr<Cober::Shader> m_Shader;
	std::shared_ptr<Cober::VertexArray> m_VertexArray;

	Cober::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;
};

class Sandbox : public Cober::Application {

public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Cober::Application* Cober::CreateApplication()
{
	return new Sandbox();
}
