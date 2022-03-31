#include "pch.h"
#include "Renderer.h"

namespace Cober {

	struct PrimitiveTypes {
		Ref<Square> square;
		Ref<Cube> cube;
	};

	static PrimitiveTypes* primitive;
	Ref<Shader> shader;

	void Renderer::Init() 
	{
		RenderCommand::Init();
		primitive = new PrimitiveTypes();

		primitive->square = CreateRef<Square>();
		primitive->cube = CreateRef<Cube>();

		shader = Shader::Create("Assets/Shaders/Texture.glsl");
		shader->Bind();
		shader->SetInt("u_Texture", 0);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		shader->Bind();
		shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		shader->SetMat4("u_View", camera.GetViewMatrix());
		shader->SetMat4("u_Model", camera.GetModelMatrix());
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		shader->Bind();
		shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		shader->SetMat4("u_View", camera.GetViewMatrix());
		shader->SetMat4("u_Model", camera.GetModelMatrix());
	}

	void Renderer::EndScene()
	{
	}

	// [-------------------- SQUARE --------------------]
	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		shader->SetFloat4("u_Color", color);
		shader->Bind();
		
		primitive->square->Draw(position, size, shader);
	}

	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		shader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		primitive->square->Draw(position, size, shader);
	}


	// [-------------------- CUBE --------------------]
	void Renderer::DrawCube(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		shader->SetFloat4("u_Color", color);
		primitive->cube->GetTexture()->Bind();

		primitive->cube->Draw(position, size, shader);
	}

	void Renderer::DrawCube(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		shader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		primitive->cube->Draw(position, size, shader);
	}
}