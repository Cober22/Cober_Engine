#pragma once
#include "API/RenderCommand.h"

#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"

#include "Cober/Renderer/Primitives/Primitives.h"
#include "Texture.h"

namespace Cober {

	class Renderer {

	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(PerspectiveCamera& camera);
		static void UploadShadersToFrustum(const Ref<Shader> shader, const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		static void EndScene();
		
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		
		// Primitives
		static void DrawSquare(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawSquare(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawSquare(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawSquare(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f));

		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f));

		static void DrawLightCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
	};
}
