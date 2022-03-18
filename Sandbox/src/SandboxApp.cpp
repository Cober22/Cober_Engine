#include <Engine.h>

#include "ImGui/imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include "Cober/Layers/LayerStack.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		Cober::Ref<Cober::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Cober::VertexBuffer::Create(vertices, sizeof(vertices)));
		Cober::BufferLayout layout = {
			{ Cober::ShaderDataType::Float3, "a_Position" },
			{ Cober::ShaderDataType::Float4, "a_Color"  }
		};
		vertexBuffer->SetLayout(layout);
		m_TriangleVAO->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Cober::Ref<Cober::IndexBuffer> indexBuffer;
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

		Cober::Ref<Cober::VertexBuffer> squareVB;
		squareVB.reset(Cober::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Cober::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVAO->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Cober::Ref<Cober::IndexBuffer> squareIB;
		squareIB.reset(Cober::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVAO->SetIndexBuffer(squareIB);


		m_ShaderTriangle.reset(Cober::Shader::Create("Shaders/triangle.vs", "Shaders/triangle.fs"));
		m_ShaderSquare.reset(Cober::Shader::Create("Shaders/square.vs", "Shaders/square.fs"));
	}

	void OnUpdate(Cober::Timestep ts) override
	{
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
		Cober::RenderCommand::Clear();
		this->ts = ts;

		m_Camera.SetPosition(m_CameraPosition);
		
		Cober::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Cober::OpenGLShader>(m_ShaderSquare)->Bind();
		std::dynamic_pointer_cast<Cober::OpenGLShader>(m_ShaderSquare)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos + m_TrianglePosition) * scale;

				/*if (x % 2 == 0)
					m_FlatColorShader->UploadUniformFloat4("u_Color", redColor);
				else
					m_FlatColorShader->UploadUniformFloat4("u_Color", blueColor);*/

				Cober::Renderer::Submit(m_ShaderSquare, m_SquareVAO, transform);
			}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
		Cober::Renderer::Submit(m_ShaderTriangle, m_TriangleVAO);

		Cober::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(SDL_Event& event) override
	{
		// --- Arrow keys
		if (event.key.keysym.sym == SDLK_LEFT) { m_CameraPosition.x += m_CameraSpeed * ts; }
		if (event.key.keysym.sym == SDLK_RIGHT) { m_CameraPosition.x -= m_CameraSpeed * ts; }
		if (event.key.keysym.sym == SDLK_DOWN) { m_CameraPosition.y += m_CameraSpeed * ts; }
		if (event.key.keysym.sym == SDLK_UP) { m_CameraPosition.y -= m_CameraSpeed * ts; }
		// --- Keyboard keys
		if (event.key.keysym.sym == SDLK_a) { m_TrianglePosition.x -= m_CameraSpeed * ts; }
		if (event.key.keysym.sym == SDLK_d) { m_TrianglePosition.x += m_CameraSpeed * ts; }
		if (event.key.keysym.sym == SDLK_s) { m_TrianglePosition.y -= m_CameraSpeed * ts; }
		if (event.key.keysym.sym == SDLK_w) { m_TrianglePosition.y += m_CameraSpeed * ts; }
	}

private:
	Cober::Ref<Cober::Shader> m_ShaderTriangle;
	Cober::Ref<Cober::VertexArray> m_TriangleVAO;
	
	Cober::Ref<Cober::Shader> m_ShaderSquare;
	Cober::Ref<Cober::VertexArray> m_SquareVAO;

	Cober::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;

	glm::vec3 m_TrianglePosition;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	Cober::Timestep ts;
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
