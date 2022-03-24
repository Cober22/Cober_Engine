#include "pch.h"
#include "Cober/Renderer/Renderer2D.h"

#include "Cober/Renderer/VertexArray.h"
#include "Cober/Renderer/Shader.h"
#include "Cober/Renderer/API/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	struct Renderer2DStorage 
	{
		Ref<VertexArray> VAO;
		Ref<Shader> Shader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
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
		s_Data = new Renderer2DStorage();
		s_Data->VAO = VertexArray::Create();

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
		s_Data->VAO->AddVertexBuffer(cubeVB);

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
		s_Data->VAO->SetIndexBuffer(cubeIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexturerData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTexturerData, sizeof(uint32_t));

		s_Data->Shader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data->Shader->Bind();
		s_Data->Shader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		s_Data->Shader->SetMat4("u_View", camera.GetViewMatrix());
	}

	void Renderer2D::BeginScene(const PerspectiveCamera& camera)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		s_Data->Shader->SetMat4("u_View", camera.GetViewMatrix());
		s_Data->Shader->SetMat4("u_Model", camera.GetModelMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	// [-------------------- QUADS --------------------]
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->Shader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();
		// Bind white texture here

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->VAO->Bind();
		RenderCommand::DrawIndexed(s_Data->VAO);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		s_Data->Shader->SetFloat4("u_Color", {0.8f, 0.2f, 0.3f, 1.0f});// glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->VAO->Bind();
		RenderCommand::DrawIndexed(s_Data->VAO);
	}
}