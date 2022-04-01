#include "pch.h"
#include "Renderer.h"

namespace Cober {

	struct PrimitiveTypes {
		Ref<Square> square;
		Ref<Cube> cube;
		Ref<LightCube> lightCube;
	};

	static PrimitiveTypes* primitive;
	Ref<Shader> shader;
	Ref<Shader> lightCubeShader;

	glm::vec3 cameraPosition;
	glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, -5.0f);	// TEST

	void Renderer::Init()
	{
		RenderCommand::Init();
		primitive = new PrimitiveTypes();

		primitive->square = CreateRef<Square>();
		primitive->cube = CreateRef<Cube>();
		primitive->lightCube = CreateRef<LightCube>();

		shader = Shader::Create("Assets/Shaders/Cube.glsl");
		shader->Bind();
		//shader->SetInt("u_Texture", 0);

		lightCubeShader = Shader::Create("Assets/Shaders/Light.glsl");
		lightCubeShader->Bind();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		cameraPosition = camera.GetPosition();
		UploadShadersToFrustum(shader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());

	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		cameraPosition = camera.GetPosition();
		UploadShadersToFrustum(shader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
	}

	void Renderer::UploadShadersToFrustum(const Ref<Shader> shader, const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model) {

		shader->Bind();
		shader->SetMat4("u_Normal", glm::mat4(glm::transpose(glm::inverse(model))));
		shader->SetMat4("u_Projection", projection);
		shader->SetMat4("u_View", view);
		shader->SetMat4("u_Model", model);
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
		shader->Bind();
		shader->SetFloat4("u_Color", color);
		shader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });
		shader->SetFloat3("u_LightPos", lightPosition);
		shader->SetFloat3("u_ViewPos", cameraPosition);
		primitive->square->GetTexture()->Bind();

		primitive->square->Draw(position, size, shader);
	}

	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		shader->Bind();
		shader->SetFloat4("u_Color", color);
		shader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });
		shader->SetFloat3("u_LightPos", lightPosition);
		shader->SetFloat3("u_ViewPos", cameraPosition);
		texture->Bind();

		primitive->square->Draw(position, size, shader);
	}


	// [-------------------- CUBE --------------------]
	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		shader->Bind();
		shader->SetFloat4("u_Color", color);
		shader->SetFloat3("u_LightColor", glm::vec3(1.0f));
		shader->SetFloat3("u_LightPos", lightPosition);
		shader->SetFloat3("u_ViewPos", cameraPosition);
		
		primitive->cube->GetTexture()->Bind();

		primitive->cube->Draw(position, size, shader);
	}

	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		shader->Bind();
		shader->SetFloat4("u_Color", color);
		shader->SetFloat3("u_LightColor", glm::vec3(1.0f));
		shader->SetFloat3("u_LightPos", lightPosition);
		shader->SetFloat3("u_ViewPos", cameraPosition);
		texture->Bind();

		primitive->cube->Draw(position, size, shader);
	}


	// [-------------------- LIGHT CUBE --------------------]
	void Renderer::DrawLightCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawLightCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		lightCubeShader->Bind();
		primitive->lightCube->Draw(position, size, lightCubeShader);
	}
}