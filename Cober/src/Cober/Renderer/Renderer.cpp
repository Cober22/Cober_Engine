#include "pch.h"
#include "Renderer.h"
#include "Cober/Renderer/Texture.h"
#include "Cober/Renderer/Shader.h"
#include "Cober/Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	struct SceneData
	{
		//Ref<OrthographicCamera> OrthoCamera;
		//Ref<PerspectiveCamera> PerspCamera;
		Ref<VertexArray> VAO;
		Ref<Shader> Shader;
		Ref<Texture2D> WhiteTexture;
	};

	static SceneData* m_SceneData;

	void Renderer::Init() 
	{
		RenderCommand::Init();
		m_SceneData = new SceneData();

		/*	s_Data = new Renderer2DStorage();
	s_Data->VAO = VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Ref<VertexBuffer> squareVB;
	squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TextCoord" }
		});
	s_Data->VAO->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<IndexBuffer> squareIB;
	squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	s_Data->VAO->SetIndexBuffer(squareIB);

	s_Data->WhiteTexture = Texture2D::Create(1, 1);
	uint32_t whiteTexturerData = 0xffffffff;
	s_Data->WhiteTexture->SetData(&whiteTexturerData, sizeof(uint32_t));

	s_Data->Shader = Shader::Create("Assets/Shaders/Texture.glsl");
	s_Data->Shader->Bind();
	s_Data->Shader->SetInt("u_Texture", 0);*/

	// [-------------------- CUBE --------------------]
		m_SceneData->VAO = VertexArray::Create();

		float cubeVertices[2 * 3 * 4] = {
			// front
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,
			 1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
			// back
			-1.0, -1.0,  1.0,
			 1.0, -1.0,  1.0,
			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0
		};

		Ref<VertexBuffer> cubeVB;
		cubeVB = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		m_SceneData->VAO->AddVertexBuffer(cubeVB);

		uint32_t cubeIndices[6 * 2 * 3] = {
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};
		Ref<IndexBuffer> cubeIB;
		cubeIB = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
		m_SceneData->VAO->SetIndexBuffer(cubeIB);

		m_SceneData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexturerData = 0xffffffff;
		m_SceneData->WhiteTexture->SetData(&whiteTexturerData, sizeof(uint32_t));

		m_SceneData->Shader = Shader::Create("Assets/Shaders/Texture.glsl");
		m_SceneData->Shader->Bind();
		m_SceneData->Shader->SetInt("u_Texture", 0);
	}

	void Renderer::Shutdown()
	{
		delete m_SceneData;
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->Shader->Bind();
		m_SceneData->Shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		m_SceneData->Shader->SetMat4("u_View", camera.GetViewMatrix());
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		m_SceneData->Shader->Bind();
		m_SceneData->Shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		m_SceneData->Shader->SetMat4("u_View", camera.GetViewMatrix());
		m_SceneData->Shader->SetMat4("u_Model", camera.GetModelMatrix());
	}

	void Renderer::EndScene()
	{
	}

	/*void Renderer::Submit(const Ref<Shader>& shader, 
						  const Ref<VertexArray>& vertexArray, 
						  const glm::mat4& transform) {

		m_SceneData->Shader->Bind();
		m_SceneData->Shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		m_SceneData->Shader->SetMat4("u_Transform", transform);
		
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}*/


	// [-------------------- QUADS --------------------]
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		m_SceneData->Shader->SetFloat4("u_Color", color);
		m_SceneData->WhiteTexture->Bind();
		// Bind white texture here

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_SceneData->Shader->SetMat4("u_Transform", transform);

		m_SceneData->VAO->Bind();
		RenderCommand::DrawIndexed(m_SceneData->VAO);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		m_SceneData->Shader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_SceneData->Shader->SetMat4("u_Transform", transform);

		m_SceneData->VAO->Bind();
		RenderCommand::DrawIndexed(m_SceneData->VAO);
	}
}