#pragma once

#include "Cober/Renderer/API/RenderCommand.h"

#include "Cober/Renderer/Camera/OrthographicCamera.h"
#include "Cober/Renderer/Camera/PerspectiveCamera.h"

#include "Cober/Renderer/Texture.h"


namespace Cober {

	class Renderer {

	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		//static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
	};
}
