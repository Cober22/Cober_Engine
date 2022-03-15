#include <Engine.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Cober::Layer
{
public:
	ExampleLayer() 
		: Layer("Example")//, m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Cober::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.33f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.33f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,   0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
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

		m_Shader.reset(new Cober::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
		Cober::RenderCommand::Clear();

		//m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
		//m_Camera.SetRotation(45.0f);

		Cober::Renderer::BeginScene();

		m_Shader->Bind();
		Cober::Renderer::Submit(m_VertexArray);

		Cober::Renderer::EndScene();
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

	//Cober::OrtographicCamera m_Camera;
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
