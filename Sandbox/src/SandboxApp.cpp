#include <Engine.h>

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Cober::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_TrianglePosition(0.0f)
	{
		// [---------- TRIANGLE ----------]
		m_TriangleVAO.reset(Cober::VertexArray::Create());

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
		m_TriangleVAO->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Cober::IndexBuffer> indexBuffer;
		indexBuffer.reset(Cober::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVAO->SetIndexBuffer(indexBuffer);


		// [---------- SQUARE ----------]
		m_SquareVAO.reset(Cober::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Cober::VertexBuffer> squareVB;
		squareVB.reset(Cober::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Cober::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVAO->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Cober::IndexBuffer> squareIB;
		squareIB.reset(Cober::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVAO->SetIndexBuffer(squareIB);


		m_ShaderTriangle.reset(Cober::Shader::Create("Shaders/triangle.vs", "Shaders/triangle.fs"));
		m_ShaderSquare.reset(Cober::Shader::Create("Shaders/square.vs", "Shaders/square.fs"));
	}

	void OnUpdate(Cober::Timestep ts) override
	{
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
		Cober::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		//m_Camera.SetRotation(45.0f);

		Cober::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++)
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos + m_TrianglePosition) * scale;
				Cober::Renderer::Submit(m_ShaderSquare, m_SquareVAO, transform);
			}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
		Cober::Renderer::Submit(m_ShaderTriangle, m_TriangleVAO);

		Cober::Renderer::EndScene();

		// [----------- INTPUT -----------]
		SDL_Event event;

		// Dispatcher events
		while (SDL_PollEvent(&event)) {
			// --- Arrow keys
			if (event.key.keysym.sym == SDLK_LEFT)	{ m_CameraPosition.x += m_CameraSpeed * ts; }
			if (event.key.keysym.sym == SDLK_RIGHT) { m_CameraPosition.x -= m_CameraSpeed * ts; }
			if (event.key.keysym.sym == SDLK_DOWN)	{ m_CameraPosition.y += m_CameraSpeed * ts; }
			if (event.key.keysym.sym == SDLK_UP)	{ m_CameraPosition.y -= m_CameraSpeed * ts; }
			// --- Keyboard keys
			if (event.key.keysym.sym == SDLK_a)	{ m_TrianglePosition.x -= m_CameraSpeed * ts; }
			if (event.key.keysym.sym == SDLK_d) { m_TrianglePosition.x += m_CameraSpeed * ts; }
			if (event.key.keysym.sym == SDLK_s)	{ m_TrianglePosition.y -= m_CameraSpeed * ts; }
			if (event.key.keysym.sym == SDLK_w)	{ m_TrianglePosition.y += m_CameraSpeed * ts; }
		}
	}

	virtual void OnImGuiRender() override 
	{
	}

	void OnEvent(SDL_Event& event) override
	{
	}

private:
	std::shared_ptr<Cober::Shader> m_ShaderTriangle;
	std::shared_ptr<Cober::VertexArray> m_TriangleVAO;

	std::shared_ptr<Cober::Shader> m_ShaderSquare;
	std::shared_ptr<Cober::VertexArray> m_SquareVAO;

	Cober::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;

	glm::vec3 m_TrianglePosition;
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
