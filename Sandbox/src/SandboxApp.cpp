#include <Engine.h>
#include <Cober/EntryPoint.h>

#include "Platforms/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Cober::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		// [---------- TRIANGLE ----------]
		m_TriangleVAO = Cober::VertexArray::Create();

		float vertices[3 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Cober::Ref<Cober::VertexBuffer> vertexBuffer;
		vertexBuffer = Cober::VertexBuffer::Create(vertices, sizeof(vertices));
		Cober::BufferLayout layout = {
			{ Cober::ShaderDataType::Float3, "a_Position" },
			{ Cober::ShaderDataType::Float2, "a_TextCoord" },
		};
		vertexBuffer->SetLayout(layout);
		m_TriangleVAO->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Cober::Ref<Cober::IndexBuffer> indexBuffer;
		indexBuffer = Cober::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_TriangleVAO->SetIndexBuffer(indexBuffer);

		// [---------- SQUARE ----------]
		m_SquareVAO = Cober::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Cober::Ref<Cober::VertexBuffer> squareVB;
		squareVB = Cober::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Cober::ShaderDataType::Float3, "a_Position" },
			{ Cober::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVAO->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Cober::Ref<Cober::IndexBuffer> squareIB;
		squareIB = Cober::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVAO->SetIndexBuffer(squareIB);


		m_ShaderTriangle = Cober::Shader::Create("Assets/Shaders/Triangle.glsl");
		m_ShaderSquare = Cober::Shader::Create("Assets/Shaders/Square.glsl");
		auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");

		m_TextureAgua = Cober::Texture2D::Create("Assets/Textures/agua.png");
		m_TextureCat = Cober::Texture2D::Create("Assets/Textures/BlendTest.png");

		std::dynamic_pointer_cast<Cober::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Cober::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Cober::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
		Cober::RenderCommand::Clear(); 
		
		Cober::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Cober::OpenGLShader>(m_ShaderSquare)->Bind();
		std::dynamic_pointer_cast<Cober::OpenGLShader>(m_ShaderSquare)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Cober::Renderer::Submit(m_ShaderSquare, m_SquareVAO, transform);
			}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_TextureAgua->Bind();
		//m_TextureCat->Bind();
		Cober::Renderer::Submit(textureShader, m_TriangleVAO, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
		//m_CameraController.OnEvent(event);
	}

private:
	// Render
	Cober::Ref<Cober::VertexArray> m_TriangleVAO, m_SquareVAO;
	Cober::Ref<Cober::Shader> m_ShaderTriangle, m_ShaderSquare;
	Cober::Ref<Cober::Texture2D> m_TextureAgua, m_TextureCat;
	// Render - Shader
	Cober::ShaderLibrary m_ShaderLibrary;
	// Render - Attributes
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
	
	// Camera
	Cober::OrthographicCameraController m_CameraController;
	
	// Timer
	Cober::Timestep ts;
};

class Sandbox : public Cober::Application {

public:
	Sandbox() 
	{
		PushLayer(new Sandbox2D());
		//PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Cober::Application* Cober::CreateApplication()
{
	return new Sandbox();
}
