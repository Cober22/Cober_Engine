#include "checkML.h"
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

		primitive.cube = CreateRef<Cube>();
		primitive.lightCube = CreateRef<LightCube>();

		basicShader = Shader::Create("Assets/Shaders/Primitive.glsl");
		basicShader->Bind();
		basicShader->SetInt("material.diffuse", 0);
		basicShader->SetInt("material.specular", 1);
		
		lightCubeShader = Shader::Create("Assets/Shaders/Light.glsl");
		lightCubeShader->Bind();
		
		//modelShader = Shader::Create("Assets/Shaders/LoadModel.glsl");
		//modelShader->Bind();

		primitive.quad = new Quad();
		baseQuadAttributes = new Quad::Attributes[Quad::maxVertices];
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
		//basicShader->SetVec3("u_ViewPos", cameraPosition);
		// Material properties
		basicShader->SetVec3("material.diffuse", { 1.0f, 1.0f, 1.0f });
		basicShader->SetFloat("material.shininess", 32.0f);
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		CB_PROFILE_FUNCTION();

		//cameraPosition = camera.GetPosition();

		SetupBasicPrimitiveShader();
		basicShader->Bind();
		basicShader->SetMat4("u_Projection", camera.GetProjection());
		basicShader->SetMat4("u_View", glm::inverse(transform));
		
		lightCubeShader->Bind();
		lightCubeShader->SetMat4("u_Projection", camera.GetProjection());
		lightCubeShader->SetMat4("u_View", glm::inverse(transform));
		
		primitive.quad->GetShader()->Bind();
		primitive.quad->GetShader()->SetMat4("u_Projection", camera.GetProjection());
		primitive.quad->GetShader()->SetMat4("u_View", glm::inverse(transform));

		// Start Batch
		primitive.quad->indexCount = 0;
		primitive.quad->attributes = baseQuadAttributes;
		primitive.quad->textureSlotIndex = 1;
	}

	void Renderer::BeginScene(const EditorCamera& camera) {

		CB_PROFILE_FUNCTION();
		//cameraPosition = camera.GetPosition();

		SetupBasicPrimitiveShader();
		basicShader->Bind();
		basicShader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		basicShader->SetMat4("u_View", camera.GetViewMatrix());

		lightCubeShader->Bind();
		lightCubeShader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		lightCubeShader->SetMat4("u_View", camera.GetViewMatrix());

		primitive.quad->GetShader()->Bind();
		primitive.quad->GetShader()->SetMat4("u_Projection", camera.GetProjectionMatrix());
		primitive.quad->GetShader()->SetMat4("u_View", camera.GetViewMatrix());
		
		// Start Batch
		primitive.quad->indexCount = 0;
		primitive.quad->attributes = baseQuadAttributes;
		primitive.quad->textureSlotIndex = 1;
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		CB_PROFILE_FUNCTION();

		cameraPosition = camera.GetPosition();
		cameraDirection = camera.GetDirection();

		SetupBasicPrimitiveShader();
		UploadShadersToFrustum(basicShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());

		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());

		//UploadShadersToFrustum(modelShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
	
		UploadShadersToFrustum(primitive.quad->GetShader(), camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		primitive.quad->indexCount = 0;
		primitive.quad->attributes = baseQuadAttributes;
		primitive.quad->textureSlotIndex = 1;
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		CB_PROFILE_FUNCTION();
		cameraPosition = camera.GetPosition();
		cameraDirection = camera.GetDirection();

		SetupBasicPrimitiveShader();
		UploadShadersToFrustum(basicShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());

		UploadShadersToFrustum(lightCubeShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		// Models
		//UploadShadersToFrustum(modelShader, camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
		// Textures
		UploadShadersToFrustum(primitive.quad->GetShader(), camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.GetModelMatrix());
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
		CB_PROFILE_FUNCTION();
		uint32_t dataSize = (uint32_t)((uint8_t*)primitive.quad->attributes - (uint8_t*)baseQuadAttributes);
		primitive.quad->GetVBO()->SetData(baseQuadAttributes, dataSize);
		Flush();
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

	// [-------------------- MODEL --------------------]
	void Renderer::DrawModel(Ref<Mesh> model, const glm::vec3& position, const glm::vec3& size)
	{
		basicShader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		basicShader->SetMat4("u_Model", transform);
		basicShader->SetMat3("u_Normal", glm::transpose(glm::inverse(transform)));

		model->Render();
	}

	// [-------------------- QUAD --------------------]
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{	//  NOT Rotation -  NOT Texture
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		primitive.quad->GetShader()->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{	//  NOT Rotation - YES Texture
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, color, tilingFactor);
	}
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		primitive.quad->GetShader()->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, texture, color, tilingFactor);
	}
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, color, tilingFactor);
	}
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor) {
		
		primitive.quad->GetShader()->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, subtexture, color, tilingFactor);
	}
	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
		primitive.quad->Draw(transform, color);
		stats.QuadCount++;
	}
	void Renderer::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
		primitive.quad->Draw(transform, texture, color, tilingFactor);
		stats.QuadCount++;
	}
	void Renderer::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor) {
		primitive.quad->Draw(transform, subtexture, color, tilingFactor);
		stats.QuadCount++;
	}

	void Renderer::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, 0, size, color);
	}
	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color) {
		primitive.quad->GetShader()->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawRotatedQuad(transform, color);
	}
	void Renderer::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, texture, color, tilingFactor);
	}
	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
		primitive.quad->GetShader()->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawRotatedQuad(transform, texture, color, tilingFactor);
	}
	void Renderer::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, subtexture, color, tilingFactor);
	}
	void Renderer::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor) {
		primitive.quad->GetShader()->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawRotatedQuad(transform, subtexture, color, tilingFactor);
	}
	void Renderer::DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color) {
		primitive.quad->Draw(transform, color);
		stats.QuadCount++;
	}
	void Renderer::DrawRotatedQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
		primitive.quad->Draw(transform, texture, color, tilingFactor);
		stats.QuadCount++;
	}
	void Renderer::DrawRotatedQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor) {
		primitive.quad->Draw(transform, subtexture, color, tilingFactor);
		stats.QuadCount++;
	}

	void Renderer::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID) {
		if (src.Texture)
			primitive.quad->Draw(transform, src.Texture, src.Color, src.TilingFactor, entityID);
		else
			primitive.quad->Draw(transform, src.Color, entityID);
	}


	// [-------------------- CUBE --------------------]
	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec3& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color)
	{
		basicShader->Bind();
		primitive.cube->GetTexture()->Bind();
		primitive.cube->Draw(position, size, basicShader);
		stats.CubeCount++;
	}

	void Renderer::DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, const glm::vec3& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, diffuseTexture, specTexture, color);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, const glm::vec3& color)
	{
		basicShader->Bind();
		diffuseTexture->Bind(0);
		specTexture->Bind(1);
		primitive.cube->Draw(position, size, basicShader);
		stats.CubeCount++;
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
		primitive.lightCube->Draw(position, size, lightCubeShader);
		stats.LightCount++;
	}


	// [-------------------- LIGHTING --------------------]
	void BindDirectionalLight(Ref<Shader>& shader, const glm::vec3& direction, const glm::vec3& color,
							  float ambient, float diffuse) {
	
		shader->SetVec3("dirLight.direction",	direction);
		shader->SetVec3("dirLight.color",		color);
		shader->SetFloat("dirLight.ambient",	ambient);
		shader->SetFloat("dirLight.diffuse",	diffuse);
		shader->SetFloat("dirLight.specular",	1.0f);
	}

	void BindPointLight(Ref<Shader>& shader, int i, const glm::vec3& position, const glm::vec3& color,
						float ambient, float diffuse, 
						float linear, float exp) {

		std::string index = std::to_string(i);
		shader->SetVec3("pointLight[" + index + "].position",	position);
		shader->SetVec3("pointLight[" + index + "].color",		color);
		shader->SetFloat("pointLight[" + index + "].ambient",	ambient);
		shader->SetFloat("pointLight[" + index + "].diffuse",	diffuse);
		shader->SetFloat("pointLight[" + index + "].specular",	1.0f);
		shader->SetFloat("pointLight[" + index + "].constant",	1.0f);
		shader->SetFloat("pointLight[" + index + "].linear",	linear);
		shader->SetFloat("pointLight[" + index + "].quadratic", exp);
	}

	void BindSpotLight(	Ref<Shader>& shader, int i, const glm::vec3& direction, const glm::vec3& position, const glm::vec3& color,
						float cutOff, float outerCutOff, 
						float ambient, float diffuse, 
						float linear, float exp) {
		
		std::string index = std::to_string(i);
		shader->SetVec3("spotLight[" + index + "].position",	position);
		shader->SetVec3("spotLight[" + index + "].color",		color);
		shader->SetVec3("spotLight[" + index + "].direction",	direction);
		shader->SetFloat("spotLight[" + index + "].ambient",	ambient);
		shader->SetFloat("spotLight[" + index + "].diffuse",	diffuse);
		shader->SetFloat("spotLight[" + index + "].specular",	1.0f);
		shader->SetFloat("spotLight[" + index + "].cutOff",		glm::cos(glm::radians(cutOff)));
		shader->SetFloat("spotLight[" + index + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
		shader->SetFloat("spotLight[" + index + "].constant",	1.0f);
		shader->SetFloat("spotLight[" + index + "].linear",		linear);
		shader->SetFloat("spotLight[" + index + "].quadratic",	exp);
	}

	void Renderer::DrawDirectionalLight(Ref<DirectionalLight> light, bool drawCube) {

		if (drawCube)
			Renderer::DrawLightCube({0.0f, 200.0f, 0.0f}, glm::vec3(20.0f), light->Color);

		basicShader->Bind();
		BindDirectionalLight(basicShader, light->Direction, light->Color, 
							 light->AmbientIntensity, light->DiffuseIntensity);
		primitive.quad->GetShader()->Bind();
		BindDirectionalLight(primitive.quad->GetShader(), light->Direction, light->Color,
							 light->AmbientIntensity, light->DiffuseIntensity);
	}
	void Renderer::DrawPointLights(std::vector<Ref<PointLight>> pointLight, bool drawCube) {

		int i = 0;
		for each (Ref<PointLight> light in pointLight) {
			if (drawCube)
				Renderer::DrawLightCube(light->Position, glm::vec3(0.5f), light->Color);

			basicShader->Bind();
			BindPointLight(	basicShader, i,
							light->Position, light->Color, 
							light->AmbientIntensity, light->DiffuseIntensity,	
							light->Attenuation.Linear, light->Attenuation.Exp);
			primitive.quad->GetShader()->Bind();
			BindPointLight(	primitive.quad->GetShader(), i,
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
			if (drawCube)
				Renderer::DrawLightCube(light->Position, glm::vec3(0.5f), light->Color);
			
			primitive.quad->GetShader()->Bind();
			BindSpotLight(	primitive.quad->GetShader(), i,
							light->Direction, light->Position, light->Color,
							light->CutOff, light->OuterCutOff,
							light->AmbientIntensity, light->DiffuseIntensity,
							light->Attenuation.Linear, light->Attenuation.Exp);
			i++;

			// Spotlight as a Flashlight
			//basicShader->Bind();
			//BindSpotLight(	basicShader, i,
			//				light->Direction, light->Position, light->Color,
			//				light->CutOff, light->OuterCutOff,
			//				light->AmbientIntensity, light->DiffuseIntensity,
			//				light->Attenuation.Linear, light->Attenuation.Exp);
		}
		basicShader->SetInt("NUM_SPOT_LIGHTS", spotLight.size());
	}
}