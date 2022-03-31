#pragma once
#include "API/RenderCommand.h"

#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"

#include "Texture.h"
#include "Cober/Renderer/Primitives/Primitives.h"

namespace Cober {

	class Renderer {

	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		
		// Primitives
		static void DrawSquare(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawSquare(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawSquare(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture);
		static void DrawSquare(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture);

		static void DrawCube(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawCube(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture);
		static void DrawCube(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture);
	};
}
