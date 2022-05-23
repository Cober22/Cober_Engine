#pragma once
#include "RenderCommand.h"

#include "Camera/EditorCamera.h"
#include "Cober/Scene/Components.h"

#include "Lighting.h"
#include "Primitives/Primitives.h"
#include "Primitives/Mesh.h"
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
			Ref<Mesh> mesh;
			Ref<LightCube> lightCube;
			//Ref<Sphere> sphere;
			std::vector<Ref<MaterialComponent>> materials;
			std::vector<Ref<Shader>> shaders;

			//Ref<DirectionalLight> dirLights;
			//std::vector<Ref<PointLight>> pointLights;
			//std::vector<Ref<SpotLight>> spotLights;
		};
		inline static PrimitiveType primitive{ PrimitiveType() };
	
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);

		static void UploadShaderToFrustum(Ref<Shader>& shader, const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		static void EndScene();
		static void Shutdown();

		static void Flush();
		static void FlushAndReset();
		
		// Primitives
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, Ref<Shader> shader, int entityID = -1);

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, Ref<Shader> shader = nullptr);
		//static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, Ref<Shader>& shader);
		//static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& diffuseTexture, const Ref<Texture2D>& specTexture, Ref<Shader>& shader);
		
		static void DrawModel(Ref<Mesh> model, const glm::vec3& position, const glm::vec3& size, Ref<Shader> shader = nullptr);
	
		static void DrawLightCube(const glm::vec3 position, const glm::vec3 size, const glm::vec3 color = glm::vec3(1.0f));

		//static void DrawDirectionalLight(Ref<DirectionalLight> light, bool drawCube, Ref<Shader>& shader);
		//static void DrawPointLight(Ref<PointLight> light, bool drawCube, Ref<Shader>& shader);
		//static void DrawSpotLight(Ref<SpotLight> light, bool drawCube, Ref<Shader>& shader);

		// Lighting
		static void BindDirectionalLight(Ref<Shader> shader, DirectionalLight& light, int i);
		static void BindPointLight(Ref<Shader> shader, PointLight& light, int i);
		static void BindSpotLight(Ref<Shader> shader, SpotLight& light, int i);

		static void Renderer::UnbindDirectionalLight(Ref<Shader> shader, DirectionalLight& light, int i) { shader->Unbind(); }
		static void Renderer::UnbindPointLight(Ref<Shader> shader, PointLight& light, int i) { shader->Unbind(); }
		static void Renderer::UnbindSpotLight(Ref<Shader> shader, SpotLight& light, int i) { shader->Unbind(); }

	};
}
