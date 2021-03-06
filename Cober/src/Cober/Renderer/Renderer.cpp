#include "pch.h"
#include "Renderer.h"

namespace Cober {

	Ref<Shader> basicShader;
	Ref<Shader> lightCubeShader;
	Ref<Shader> modelShader;
	Ref<Shader> textureShader;

	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;
	glm::vec3 l_Color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 l_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 l_Dir = glm::vec3(-0.2f, -1.0f, -0.3f);	
	Quad::Attributes* baseQuadAttributes;

	void Renderer::Init()
	{
		RenderCommand::Init();

		for (int i = 0; i < primitive.materials.size(); i++) {
			if (primitive.materials[i]->shader) {
				primitive.materials[i]->shader->Bind();
			}
		}

		primitive.cube = CreateRef<Cube>();
		primitive.mesh = CreateRef<Mesh>();
		primitive.lightCube = CreateRef<LightCube>();
		primitive.lightCube->GetShader()->Bind();
		
		primitive.quad = new Quad();
		baseQuadAttributes = new Quad::Attributes[Quad::maxVertices];

		modelShader = CreateRef<Shader>("Assets/Shaders/LoadModel.glsl");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		CB_PROFILE_FUNCTION();

		for (int i = 0; i < primitive.materials.size(); i++) {
			if (primitive.materials[i]->shader) {
				primitive.materials[i]->shader->Bind();
				primitive.materials[i]->shader->SetMat4("u_Projection", camera.GetProjection());
				primitive.materials[i]->shader->SetMat4("u_View", glm::inverse(transform));
				// Extract to material shader properties
				primitive.materials[i]->shader->SetVec3("material.diffuse", { 1.0f, 1.0f, 1.0f });
				primitive.materials[i]->shader->SetFloat("material.shininess", 32.0f);
			}
		}
		primitive.lightCube->GetShader()->Bind();
		primitive.lightCube->GetShader()->SetMat4("u_Projection", camera.GetProjection());
		primitive.lightCube->GetShader()->SetMat4("u_View", glm::inverse(transform));

		// Start Batch
		primitive.quad->GetShader()->Bind();
		primitive.quad->GetShader()->SetMat4("u_Projection", camera.GetProjection());
		primitive.quad->GetShader()->SetMat4("u_View", glm::inverse(transform));
		primitive.quad->indexCount = 0;
		primitive.quad->attributes = baseQuadAttributes;
		primitive.quad->textureSlotIndex = 1;
	}

	void Renderer::BeginScene(const EditorCamera& camera) {


		CB_PROFILE_FUNCTION();

		for (int i = 0; i < primitive.materials.size(); i++) {
			if (primitive.materials[i]->shader) {
				primitive.materials[i]->shader->Bind();
				primitive.materials[i]->shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
				primitive.materials[i]->shader->SetMat4("u_View", camera.GetViewMatrix());
				// Extract to material shader properties
				primitive.materials[i]->shader->SetVec3("material.diffuse", { 1.0f, 1.0f, 1.0f });
				primitive.materials[i]->shader->SetFloat("material.shininess", 32.0f);
			}
		}
		primitive.lightCube->GetShader()->Bind();
		primitive.lightCube->GetShader()->SetMat4("u_Projection", camera.GetProjectionMatrix());
		primitive.lightCube->GetShader()->SetMat4("u_View", camera.GetViewMatrix());
		// Start Batch
		//primitive.quad->GetVAO()->Bind();
		//primitive.quad->GetShader()->Bind();
		//primitive.quad->GetShader()->SetMat4("u_Projection", camera.GetProjectionMatrix());
		//primitive.quad->GetShader()->SetMat4("u_View", camera.GetViewMatrix());
		primitive.quad->indexCount = 0;
		primitive.quad->attributes = baseQuadAttributes;
		primitive.quad->textureSlotIndex = 1;
	}	
	
	void Renderer::Shutdown() 
	{
		CB_PROFILE_FUNCTION();
		//delete[] primitive.quad->attributes;
	}

	void Renderer::EndScene()
	{
		//CB_PROFILE_FUNCTION();
		uint32_t dataSize = (uint32_t)((uint8_t*)primitive.quad->attributes - (uint8_t*)baseQuadAttributes);
		primitive.quad->GetVBO()->SetData(baseQuadAttributes, dataSize);
		//Flush();
	}

	void Renderer::Flush() {

		if (primitive.quad->indexCount == 0)
			return; // Nothing to draw
		
		// Bind textures
		for (uint32_t i = 0; i < primitive.quad->textureSlotIndex; i++)
			primitive.quad->textureSlots[i]->Bind(i);
		primitive.quad->GetVAO()->Bind();
		RenderCommand::DrawIndexed(primitive.quad->GetVAO(), primitive.quad->indexCount);
		stats.DrawCalls++;
	}

	void Renderer::FlushAndReset() {
		
		EndScene();
		
		primitive.quad->indexCount = 0;
		primitive.quad->attributes = baseQuadAttributes;
		
		primitive.quad->textureSlotIndex = 1;
	}

	void Renderer::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, Ref<Shader> shader, int entityID) {
		if (src.Texture)
			primitive.quad->Draw(transform, src.Texture, shader, src.Color, src.TilingFactor, entityID);
		else
			primitive.quad->Draw(transform, src.Color, shader, entityID);
		stats.QuadCount++;
		Flush();
	}

	void Renderer::DrawCube(const glm::mat4& transform, Ref<Shader> shader)
	{
		if (shader)
			shader->Bind();
		else
			primitive.cube->GetShader()->Bind();
		primitive.cube->GetTexture()->Bind();
		primitive.cube->Draw(transform, shader);
		stats.CubeCount++;
	}

	// [-------------------- LIGHT CUBE --------------------]
	void Renderer::DrawLightCube(const glm::vec3 position, const glm::vec3 size, const glm::vec3 color)
	{
		if (primitive.lightCube->GetShader()) {
			primitive.lightCube->GetShader()->Bind();
			primitive.lightCube->GetTexture()->Bind();
			primitive.lightCube->Draw(position, size, color);
			stats.CubeCount++;
		}
	}

	// [-------------------- MODEL --------------------]
	void Renderer::DrawModel(Ref<Mesh> model, const glm::mat4& transform, Ref<Shader> shader)
	{
		if (shader) {
			shader->Bind();
			shader->SetMat4("u_Model", transform);
			shader->SetMat3("u_Normal", glm::transpose(glm::inverse(transform)));
			model->Render();
		}
		else {
			modelShader->Bind();
			modelShader->SetMat4("u_Model", transform);
			modelShader->SetMat3("u_Normal", glm::transpose(glm::inverse(transform)));
			model->Render();
		}
	}

	// [-------------------- LIGHTING --------------------]
	void Renderer::BindDirectionalLight(Ref<Shader> shader, DirectionalLight& light) {

		shader->Bind();
		shader->SetVec3("dirLight.direction", light.Direction);
		shader->SetVec3("dirLight.color", light.Color);
		shader->SetFloat("dirLight.ambient", light.AmbientIntensity);
		shader->SetFloat("dirLight.diffuse", light.DiffuseIntensity);
		shader->SetFloat("dirLight.specular", 0.0f);
	}

	void Renderer::BindPointLight(Ref<Shader> shader, PointLight& light, int i) {

		shader->Bind();
		std::string index = std::to_string(i);
		shader->SetVec3("pointLight[" + index + "].position", light.Position);
		shader->SetVec3("pointLight[" + index + "].color", light.Color);
		shader->SetFloat("pointLight[" + index + "].ambient", light.AmbientIntensity);
		shader->SetFloat("pointLight[" + index + "].diffuse", light.DiffuseIntensity);
		shader->SetFloat("pointLight[" + index + "].specular", 1.0f);
		shader->SetFloat("pointLight[" + index + "].constant", 1.0f);
		shader->SetFloat("pointLight[" + index + "].linear", light.Attenuation.Linear);
		shader->SetFloat("pointLight[" + index + "].quadratic", light.Attenuation.Exp);
	}

	void Renderer::BindSpotLight(Ref<Shader> shader, SpotLight& light, int i) {

		shader->Bind();
		std::string index = std::to_string(i);
		shader->SetVec3("spotLight[" + index + "].position", light.Position);
		shader->SetVec3("spotLight[" + index + "].color", light.Color);
		shader->SetVec3("spotLight[" + index + "].direction", light.Direction);
		shader->SetFloat("spotLight[" + index + "].ambient", light.AmbientIntensity);
		shader->SetFloat("spotLight[" + index + "].diffuse", light.DiffuseIntensity);
		shader->SetFloat("spotLight[" + index + "].specular", 1.0f);
		shader->SetFloat("spotLight[" + index + "].cutOff", glm::cos(glm::radians(light.CutOff)));
		shader->SetFloat("spotLight[" + index + "].outerCutOff", glm::cos(glm::radians(light.OuterCutOff)));
		shader->SetFloat("spotLight[" + index + "].constant", 1.0f);
		shader->SetFloat("spotLight[" + index + "].linear", light.Attenuation.Linear);
		shader->SetFloat("spotLight[" + index + "].quadratic", light.Attenuation.Exp);
	}
}