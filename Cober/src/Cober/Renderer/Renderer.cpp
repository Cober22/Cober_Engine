#include "pch.h"
#include "Renderer.h"

namespace Cober {

	struct PrimitiveType {
		Ref<Square> square;
		Ref<Cube> cube;
		Ref<LightCube> lightCube;
	};

	static PrimitiveType* primitive;
	Ref<Shader> basePrimitiveShader;
	Ref<Shader> lightCubeShader;

	glm::vec3 cameraPosition;
	glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, -5.0f);	// TEST
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);	// TEST

	void Renderer::Init()
	{
		RenderCommand::Init();

		primitive = new PrimitiveType();
		primitive->square = CreateRef<Square>();
		primitive->cube = CreateRef<Cube>();
		primitive->lightCube = CreateRef<LightCube>();

		basePrimitiveShader = Shader::Create("Assets/Shaders/Primitive.glsl");
		basePrimitiveShader->Bind();
		basePrimitiveShader->SetInt("material.diffuse", 0);
		basePrimitiveShader->SetInt("material.specular", 1);

		lightCubeShader = Shader::Create("Assets/Shaders/Light.glsl");
		lightCubeShader->Bind();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void UploadShadersToFrustum(const Ref<Shader> shader, const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model) {

		shader->Bind();
		shader->SetMat4("u_Projection", projection);
		shader->SetMat4("u_View", view);
		shader->SetMat4("u_Model", model);
		// Calculate the normal matrix on the CPU and send it to the GPU because inversing matrices is a costly operation for shaders
		shader->SetMat3("u_Normal", glm::transpose(glm::inverse(model)));
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		cameraPosition = camera.GetPosition();
		UploadShadersToFrustum(basePrimitiveShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());

	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		cameraPosition = camera.GetPosition();
		UploadShadersToFrustum(basePrimitiveShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
	}

	void Renderer::EndScene()
	{
	}

	void SetupBasicPrimitiveShader(const glm::vec4& color) {

		basePrimitiveShader->Bind();
		basePrimitiveShader->SetFloat4("u_Color", color);
		basePrimitiveShader->SetFloat3("u_ViewPos", cameraPosition);
		basePrimitiveShader->SetFloat3("light.position", lightPosition);
		basePrimitiveShader->SetFloat3("light.specular", lightColor);

		// ONLY FOR TEST
		//	/*
		basePrimitiveShader->SetFloat3("light.ambient", { 0.2f, 0.2f, 0.2f });
		basePrimitiveShader->SetFloat3("light.diffuse", { 0.5f, 0.5f, 0.5f }); // darkened
		//	*/ 

		basePrimitiveShader->SetFloat("material.shininess", 64.0f);
	}

	// [-------------------- SQUARE --------------------]
	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		SetupBasicPrimitiveShader(color);
		primitive->square->GetTexture()->Bind();
		primitive->square->Draw(position, size, basePrimitiveShader);
	}

	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		SetupBasicPrimitiveShader(color);
		texture->Bind(0);
		texture->Bind(1);	// Provisional from delete the specular map from the cat Texture
		primitive->square->Draw(position, size, basePrimitiveShader);
	}


	// [-------------------- CUBE --------------------]
	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		SetupBasicPrimitiveShader(color);
		primitive->cube->GetTexture()->Bind();
		primitive->cube->Draw(position, size, basePrimitiveShader);
	}

	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D> diffuseTexture, const Ref<Texture2D> specTexture, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, diffuseTexture, specTexture, color);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D> diffuseTexture, const Ref<Texture2D> specTexture, const glm::vec4& color)
	{
		SetupBasicPrimitiveShader(color);
		diffuseTexture->Bind(0);
		specTexture->Bind(1);
		primitive->cube->Draw(position, size, basePrimitiveShader);
	}


	// [-------------------- LIGHT CUBE --------------------]
	void Renderer::DrawLightCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawLightCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		lightColor = color;
		lightCubeShader->Bind();
		lightCubeShader->SetFloat4("u_Color", color);
		primitive->lightCube->Draw(position, size, lightCubeShader);
	}
}