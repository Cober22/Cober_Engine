#pragma once
#include "API/RenderCommand.h"

#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "Camera/Camera.h"
#include "Camera/EditorCamera.h"
#include "Cober/Scene/Components.h"

#include "Lighting.h"
#include "Primitives/Primitives.h"
#include "Primitives/Mesh.h"
#include "SubTexture2D.h"
#include "SubTexture2D.h"
#include "Texture.h"


namespace Cober {

	class Renderer {

	public:
		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t CubeCount = 0;
			uint32_t LightCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4 + CubeCount * 8 + LightCount * 8; };
			uint32_t GetTotalIndexCount() { return QuadCount * 6 + CubeCount * 6 * 2 * 3 + LightCount * 6 * 2 * 3; };
		};
		inline static Stats stats{ Stats() };
		inline static void ResetStats() { memset(&stats, 0, sizeof(Stats)); };
		inline static Stats GetStats() { return stats; };
	
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
	
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
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

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID = -1);

		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec3& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);
		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, const glm::vec3& color = glm::vec3(1.0f));
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, const glm::vec3& color = glm::vec3(1.0f));
		static void DrawModel(Ref<Mesh> model, const glm::vec3& position = {0.0f, 0.0f, 0.0f}, const glm::vec3& size = { 1.0f, 1.0f, 1.0f });
	
		static void DrawLightCube(const glm::vec2& position, const glm::vec3& size, const glm::vec3& color = glm::vec3(1.0f));
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color = glm::vec3(1.0f));

		// Ligting
		static void DrawDirectionalLight(Ref<DirectionalLight>	 light, bool drawCube = false);
		static void DrawPointLights(std::vector<Ref<PointLight>> light, bool drawCube = false);
		static void DrawSpotLights(std::vector<Ref<SpotLight>>	 light, bool drawCube = false);
	};
}
