#pragma once
#include "API/RenderCommand.h"

#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"

#include "Cober/Renderer/Lighting.h"
#include "Cober/Renderer/Primitives/Primitives.h"
#include "Cober/Renderer/Primitives/Mesh.h"
#include "SubTexture2D.h"
#include "Texture.h"


namespace Cober {

	class Renderer {

	public:
		struct PrimitiveType {
			Quad* quad;
			Ref<Cube> cube;
			Ref<LightCube> lightCube;

			Ref<DirectionalLight> dirLight;
			std::vector<Ref<PointLight>> pointLights;
			std::vector<Ref<SpotLight>> spotLights;
		};
		inline static PrimitiveType primitive{ PrimitiveType() };

		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void ResetStats();
		static Ref<Stats> GetStats();
		
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(PerspectiveCamera& camera);
		static void UploadShaderToFrustum(const Ref<Shader> shader, const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		static void EndScene();
		static void Shutdown();

		static void Flush();
		static void FlushAndReset();
		
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec3& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);
		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, const glm::vec3& color = glm::vec3(1.0f));
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, const glm::vec3& color = glm::vec3(1.0f));
		//static void DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f));
		//static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawModel(Ref<Mesh> model, const glm::vec3& position = {0.0f, 0.0f, 0.0f}, const glm::vec3& size = { 1.0f, 1.0f, 1.0f });
	
		static void DrawLightCube(const glm::vec2& position, const glm::vec3& size, const glm::vec3& color = glm::vec3(1.0f));
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color = glm::vec3(1.0f));

		// Ligting
		static void DrawDirectionalLight(Ref<DirectionalLight>	 light, bool drawCube = false);
		static void DrawPointLights(std::vector<Ref<PointLight>> light, bool drawCube = false);
		static void DrawSpotLights(std::vector<Ref<SpotLight>>	 light, bool drawCube = false);

	};
}
