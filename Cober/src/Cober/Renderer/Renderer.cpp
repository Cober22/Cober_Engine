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
		//basePrimitiveShader->SetVec3("material.diffuse", {1.0f, 1.0f, 1.0f});
		basePrimitiveShader->SetFloat("material.shininess", 32.0f);

		/////////////////////////// TEST
		// Directional light
		basePrimitiveShader->SetVec3("dirLight.color",		glm::vec3(1.0f, 1.0f, 1.0f));
		basePrimitiveShader->SetVec3("dirLight.direction",	glm::vec3(-0.2f, -1.0f, -0.3f));
		basePrimitiveShader->SetVec3("dirLight.ambient",	glm::vec3(0.0f));
		basePrimitiveShader->SetVec3("dirLight.diffuse",	glm::vec3(0.0f));
		basePrimitiveShader->SetVec3("dirLight.specular",	glm::vec3(0.0f));

		// Point Light
		basePrimitiveShader->SetVec3("pointLight[0].color", glm::vec3(0.0f, 1.0f, 0.0f));
		basePrimitiveShader->SetVec3("pointLight[0].position", glm::vec3(10.7f, 0.2f, -2.0f));
		basePrimitiveShader->SetVec3("pointLight[0].ambient", glm::vec3(1.0f));
		basePrimitiveShader->SetVec3("pointLight[0].diffuse", glm::vec3(1.0f));
		basePrimitiveShader->SetVec3("pointLight[0].specular", glm::vec3(1.0f));
		basePrimitiveShader->SetFloat("pointLight[0].constant", 1.0f);
		basePrimitiveShader->SetFloat("pointLight[0].linear", 0.2f);
		basePrimitiveShader->SetFloat("pointLight[0].quadratic", 0.1f);

		basePrimitiveShader->SetVec3("pointLight[1].color", glm::vec3(0.0f, 0.0f, 1.0f));
		basePrimitiveShader->SetVec3("pointLight[1].position", glm::vec3(2.3f, 3.3f, -44.0f));
		basePrimitiveShader->SetVec3("pointLight[1].ambient", glm::vec3(1.0f));
		basePrimitiveShader->SetVec3("pointLight[1].diffuse", glm::vec3(1.0f));
		basePrimitiveShader->SetVec3("pointLight[1].specular", glm::vec3(1.0f));
		basePrimitiveShader->SetFloat("pointLight[1].constant", 1.0f);
		basePrimitiveShader->SetFloat("pointLight[1].linear", 0.2f);
		basePrimitiveShader->SetFloat("pointLight[1].quadratic", 0.1f);

		basePrimitiveShader->SetVec3("pointLight[2].color", glm::vec3(1.0f, 0.0f, 0.0f));
		basePrimitiveShader->SetVec3("pointLight[2].position", glm::vec3(-4.0f, 2.0f, -12.0f));
		basePrimitiveShader->SetVec3("pointLight[2].ambient", glm::vec3(1.0f));
		basePrimitiveShader->SetVec3("pointLight[2].diffuse", glm::vec3(1.0f));
		basePrimitiveShader->SetVec3("pointLight[2].specular", glm::vec3(1.0f));
		basePrimitiveShader->SetFloat("pointLight[2].constant", 1.0f);
		basePrimitiveShader->SetFloat("pointLight[2].linear", 0.2);
		basePrimitiveShader->SetFloat("pointLight[2].quadratic", 0.1f);

		basePrimitiveShader->SetVec3("pointLight[3].color", glm::vec3(1.0f, 0.0f, 1.0f));
		basePrimitiveShader->SetVec3("pointLight[3].position", glm::vec3(0.0f, 0.0f, -30.0f));
		basePrimitiveShader->SetVec3("pointLight[3].ambient", glm::vec3(1.0f));
		basePrimitiveShader->SetVec3("pointLight[3].diffuse", glm::vec3(1.0));
		basePrimitiveShader->SetVec3("pointLight[3].specular", glm::vec3(1.0));
		basePrimitiveShader->SetFloat("pointLight[3].constant", 1.0f);
		basePrimitiveShader->SetFloat("pointLight[3].linear", 0.2f);
		basePrimitiveShader->SetFloat("pointLight[3].quadratic", 0.1f);

		// Spot Lights
		basePrimitiveShader->SetVec3("spotLight.color",		glm::vec3(1.0f, 1.0f, 0.0f));
		basePrimitiveShader->SetVec3("spotLight.position",	cameraPosition);
		basePrimitiveShader->SetVec3("spotLight.direction", cameraDirection);
		basePrimitiveShader->SetVec3("spotLight.ambient",	glm::vec3(0.05f));
		basePrimitiveShader->SetVec3("spotLight.diffuse",	glm::vec3(0.8f));
		basePrimitiveShader->SetVec3("spotLight.specular",	glm::vec3(1.0f));
		basePrimitiveShader->SetFloat("spotLight.cutOff",	glm::cos(glm::radians(8.5f)));
		basePrimitiveShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));
		basePrimitiveShader->SetFloat("spotLight.constant", 1.0f);
		basePrimitiveShader->SetFloat("spotLight.linear",	0.09f);
		basePrimitiveShader->SetFloat("spotLight.quadratic",0.032);
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
		lightCubeShader->Bind();
		lightCubeShader->SetFloat4("u_Color", color);
		primitive->lightCube->Draw(position, size, lightCubeShader);
	}
}