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
	glm::vec3 cameraDirection;
	glm::vec3 l_Color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 l_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 l_Dir = glm::vec3(-0.2f, -1.0f, -0.3f);	
	int indexPointLight = 0;

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

	void SetupBasicPrimitiveShader() {
		
		basePrimitiveShader->Bind();
		basePrimitiveShader->SetVec3("u_ViewPos", cameraPosition);
		// Material properties
		basePrimitiveShader->SetFloat("material.shininess", 32.0f);

		/////////////////////////// TEST
		// Directional light
		basePrimitiveShader->SetVec3("dirLight.color",		glm::vec3(1.0f, 1.0f, 1.0f));
		basePrimitiveShader->SetVec3("dirLight.direction",	glm::vec3(-0.2f, -1.0f, -0.3f));
		basePrimitiveShader->SetVec3("dirLight.ambient",	glm::vec3(0.05f, 0.05f, 0.05f));
		basePrimitiveShader->SetVec3("dirLight.diffuse",	glm::vec3(0.4f, 0.4f, 0.4f));
		basePrimitiveShader->SetVec3("dirLight.specular",	glm::vec3(0.5f, 0.5f, 0.5f));

		// Spot Lights
		basePrimitiveShader->SetVec3("spotLight.color",		glm::vec3(1.0f, 1.0f, 1.0f));
		basePrimitiveShader->SetVec3("spotLight.position",	cameraPosition);
		basePrimitiveShader->SetVec3("spotLight.direction", cameraDirection);
		basePrimitiveShader->SetVec3("spotLight.ambient",	glm::vec3(0.4f, 0.4f, 0.4f));
		basePrimitiveShader->SetVec3("spotLight.diffuse",	glm::vec3(0.8f, 0.8f, 0.8f));
		basePrimitiveShader->SetVec3("spotLight.specular",	glm::vec3(1.0f, 1.0f, 1.0f));
		basePrimitiveShader->SetFloat("spotLight.cutOff",	5.0f);
		basePrimitiveShader->SetFloat("spotLight.outerCutOff", 10.0f);
		basePrimitiveShader->SetFloat("spotLight.constant", 1.0f);
		basePrimitiveShader->SetFloat("spotLight.linear",	0.1f);
		basePrimitiveShader->SetFloat("spotLight.quadratic",0.1f);
		/////////////////////////// TEST
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		cameraPosition = camera.GetPosition();
		cameraDirection = camera.GetDirection();
		SetupBasicPrimitiveShader();
		UploadShadersToFrustum(basePrimitiveShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());

	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		indexPointLight = 0;
		cameraPosition = camera.GetPosition();
		cameraDirection = camera.GetDirection();
		SetupBasicPrimitiveShader();
		UploadShadersToFrustum(basePrimitiveShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
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
		basePrimitiveShader->Bind();
		primitive->square->GetTexture()->Bind();
		primitive->square->Draw(position, size, basePrimitiveShader);
	}

	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color)
	{
		basePrimitiveShader->Bind();
		texture->Bind(0);
		texture->Bind(1);	// Provisional, delete the specular map from the cat Texture
		primitive->square->Draw(position, size, basePrimitiveShader);
	}


	// [-------------------- CUBE --------------------]
	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		basePrimitiveShader->Bind();
		primitive->cube->GetTexture()->Bind();
		primitive->cube->Draw(position, size, basePrimitiveShader);
	}

	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D> diffuseTexture, const Ref<Texture2D> specTexture, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, diffuseTexture, specTexture, color);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D> diffuseTexture, const Ref<Texture2D> specTexture, const glm::vec4& color)
	{
		basePrimitiveShader->Bind();
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
		basePrimitiveShader->Bind();

		std::string pointLight = "pointLight[" + std::to_string(indexPointLight) + "]";
		basePrimitiveShader->SetVec3(pointLight + ".color", color);
		basePrimitiveShader->SetVec3(pointLight + ".position",	position);
		basePrimitiveShader->SetVec3(pointLight + ".ambient",	glm::vec3(0.5f, 0.5f, 0.5f));
		basePrimitiveShader->SetVec3(pointLight + ".diffuse",	glm::vec3(0.8f, 0.8f, 0.8f));
		basePrimitiveShader->SetVec3(pointLight + ".specular", glm::vec3(1.0f, 1.0f, 1.0f));
		basePrimitiveShader->SetFloat(pointLight + ".constant", 1.0f);
		basePrimitiveShader->SetFloat(pointLight + ".linear",	0.1f);
		basePrimitiveShader->SetFloat(pointLight + ".quadratic",0.1f);
		if (indexPointLight == 3)
			basePrimitiveShader->SetVec3(pointLight + ".diffuse", glm::vec3(1.0f));

		indexPointLight++;

		lightCubeShader->Bind();
		lightCubeShader->SetFloat4("u_Color", color);
		primitive->lightCube->Draw(position, size, lightCubeShader);
	}
}