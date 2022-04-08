#include "pch.h"
#include "Renderer.h"

namespace Cober {

	Ref<Shader> basicShader;
	Ref<Shader> lightCubeShader;
	Ref<Shader> modelShader;

	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;
	glm::vec3 l_Color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 l_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 l_Dir = glm::vec3(-0.2f, -1.0f, -0.3f);	

	void Renderer::Init()
	{
		RenderCommand::Init();

		primitive->square = CreateRef<Square>();
		primitive->cube = CreateRef<Cube>();
		primitive->lightCube = CreateRef<LightCube>();

		basicShader = Shader::Create("Assets/Shaders/Primitive.glsl");
		basicShader->Bind();
		basicShader->SetInt("material.diffuse", 0);
		basicShader->SetInt("material.specular", 1);

		lightCubeShader = Shader::Create("Assets/Shaders/Light.glsl");
		lightCubeShader->Bind();

		modelShader = Shader::Create("Assets/Shaders/LoadModel.glsl");
		modelShader->Bind();
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
		
		basicShader->Bind();
		basicShader->SetVec3("u_ViewPos", cameraPosition);
		// Material properties
		basicShader->SetVec3("material.diffuse", { 1.0f, 1.0f, 1.0f });
		basicShader->SetFloat("material.shininess", 32.0f);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		cameraPosition = camera.GetPosition();
		cameraDirection = camera.GetDirection();
		SetupBasicPrimitiveShader();
		UploadShadersToFrustum(basicShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(modelShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		cameraPosition = camera.GetPosition();
		cameraDirection = camera.GetDirection();
		SetupBasicPrimitiveShader();
		UploadShadersToFrustum(basicShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		UploadShadersToFrustum(modelShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
	}

	void Renderer::EndScene()
	{
	}

	// [-------------------- MODEL --------------------]
	void Renderer::DrawModel(Ref<Mesh> model, const glm::vec3& position, const glm::vec3& size)
	{
		basicShader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		basicShader->SetMat4("u_Model", transform);
		model->Render();
	}

	// [-------------------- SQUARE --------------------]
	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color)
	{
		basicShader->Bind();
		primitive->square->GetTexture()->Bind();
		primitive->square->Draw(position, size, basicShader);
	}

	void Renderer::DrawSquare(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec3& color)
	{
		DrawSquare({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec3& color)
	{
		basicShader->Bind();
		texture->Bind(0);
		texture->Bind(1);	// Provisional, delete the specular map from the cat Texture
		primitive->square->Draw(position, size, basicShader);
	}


	// [-------------------- CUBE --------------------]
	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec3& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color)
	{
		basicShader->Bind();
		primitive->cube->GetTexture()->Bind();
		primitive->cube->Draw(position, size, basicShader);
	}

	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D> diffuseTexture, const Ref<Texture2D> specTexture, const glm::vec3& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, diffuseTexture, specTexture, color);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D> diffuseTexture, const Ref<Texture2D> specTexture, const glm::vec3& color)
	{
		basicShader->Bind();
		diffuseTexture->Bind(0);
		specTexture->Bind(1);
		primitive->cube->Draw(position, size, basicShader);
	}


	// [-------------------- LIGHT CUBE --------------------]
	void Renderer::DrawLightCube(const glm::vec2& position, const glm::vec3& size, const glm::vec3& color)
	{
		DrawLightCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color)
	{
		lightCubeShader->Bind();
		lightCubeShader->SetFloat3("u_Color", color);
		primitive->lightCube->Draw(position, size, lightCubeShader);
	}


	// [-------------------- LIGHTING --------------------]
	void BindDirectionalLight(const glm::vec3& direction, const glm::vec3& color, 
							  float ambient, float diffuse) {
	
		basicShader->SetVec3("dirLight.direction",	direction);
		basicShader->SetVec3("dirLight.color",		color);
		basicShader->SetFloat("dirLight.ambient",	ambient);
		basicShader->SetFloat("dirLight.diffuse",	diffuse);
		basicShader->SetFloat("dirLight.specular",	1.0f);
	}

	void BindPointLight(int i, const glm::vec3& position, const glm::vec3& color, 
						float ambient, float diffuse, 
						float linear, float exp) {

		std::string index = std::to_string(i);
		basicShader->SetVec3("pointLight[" + index + "].position", position);
		basicShader->SetVec3("pointLight[" + index + "].color", color);
		basicShader->SetFloat("pointLight[" + index + "].ambient", ambient);
		basicShader->SetFloat("pointLight[" + index + "].diffuse", diffuse);
		basicShader->SetFloat("pointLight[" + index + "].specular", 1.0f);
		basicShader->SetFloat("pointLight[" + index + "].constant", 1.0f);
		basicShader->SetFloat("pointLight[" + index + "].linear", linear);
		basicShader->SetFloat("pointLight[" + index + "].quadratic", exp);
	}

	void BindSpotLight(	int i, const glm::vec3& direction, const glm::vec3& position, const glm::vec3& color,
						float cutOff, float outerCutOff, 
						float ambient, float diffuse, 
						float linear, float exp) {
		
		std::string index = std::to_string(i);
		basicShader->SetVec3("spotLight[" + index + "].position",	position);
		basicShader->SetVec3("spotLight[" + index + "].color",		color);
		basicShader->SetVec3("spotLight[" + index + "].direction",  direction);
		basicShader->SetFloat("spotLight[" + index + "].ambient",	ambient);
		basicShader->SetFloat("spotLight[" + index + "].diffuse",	diffuse);
		basicShader->SetFloat("spotLight[" + index + "].specular",	1.0f);
		basicShader->SetFloat("spotLight[" + index + "].cutOff",	glm::cos(glm::radians(cutOff)));
		basicShader->SetFloat("spotLight[" + index + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
		basicShader->SetFloat("spotLight[" + index + "].constant",	1.0f);
		basicShader->SetFloat("spotLight[" + index + "].linear",	linear);
		basicShader->SetFloat("spotLight[" + index + "].quadratic",	exp);
	}

	void Renderer::DrawDirectionalLight(Ref<DirectionalLight> light, bool drawCube) {

		if (drawCube)
			Renderer::DrawLightCube({0.0f, 200.0f, 0.0f}, glm::vec3(20.0f), light->Color);

		basicShader->Bind();
		BindDirectionalLight(light->Direction, light->Color, 
							 light->AmbientIntensity, light->DiffuseIntensity);
	}
	void Renderer::DrawPointLights(std::vector<Ref<PointLight>> pointLight, bool drawCube) {

		int i = 0;
		for each (Ref<PointLight> light in pointLight) {
			if (drawCube)
				Renderer::DrawLightCube(light->Position, glm::vec3(0.5f), light->Color);

			basicShader->Bind();
			BindPointLight(	i,
							light->Position, light->Color, 
							light->AmbientIntensity, light->DiffuseIntensity,	
							light->Attenuation.Linear, light->Attenuation.Exp);
			i++;
		}
		basicShader->SetInt("NUM_POINT_LIGHTS", pointLight.size());
	}
	void Renderer::DrawSpotLights(std::vector<Ref<SpotLight>> spotLight, bool drawCube) {

		int i = 0;
		for each (Ref<SpotLight> light in spotLight) {
			if (drawCube && i != 0)
				Renderer::DrawLightCube(light->Position, glm::vec3(0.5f), light->Color);
			
			basicShader->Bind();
			BindSpotLight(i,
				          light->Direction, light->Position, light->Color,
				          light->CutOff, light->OuterCutOff,
				          light->AmbientIntensity, light->DiffuseIntensity,
				          light->Attenuation.Linear, light->Attenuation.Exp);
			i++;
		}
		basicShader->SetInt("NUM_SPOT_LIGHTS", spotLight.size());
	}
}