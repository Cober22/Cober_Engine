#pragma once
#include "Cober/Renderer/VertexArray.h"
#include "Cober/Renderer/Texture.h"
#include "Cober/Renderer/Shader.h"

#include "Cober/Renderer/Buffer.h"

namespace Cober {

	class Square {
	public:
		Square();
		void Draw(const glm::vec3& position, const glm::vec2& size, Ref<Shader> shader);
		void Init();
		Ref<VertexArray> GetVAO() { return VAO; }
		Ref<Texture> GetTexture() { return WhiteTexture; }
	private:
		// Primitive Attributes
		Ref<VertexArray> VAO;
		Ref<Texture2D> WhiteTexture;

		// Data
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		// Buffer Objects
		Ref<VertexBuffer> squareVB;
		Ref<IndexBuffer> squareIB;
	};

	class Cube {
	public:
		Cube();
		void Draw(const glm::vec3& position, const glm::vec2& size, Ref<Shader> shader);
		void Init();
		Ref<VertexArray> GetVAO() { return VAO; }
		Ref<Texture> GetTexture() { return WhiteTexture; }

	private:
		// Primitive Attributes
		Ref<VertexArray> VAO;
		Ref<Texture2D> WhiteTexture;

		// Data
		float cubeVertices[2 * 3 * 4] = {
			// front
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,
			 1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
			// back
			-1.0, -1.0,  1.0,
			 1.0, -1.0,  1.0,
			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0
		};
		uint32_t cubeIndices[6 * 2 * 3] = {
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};

		// Buffer Objects
		Ref<VertexBuffer> cubeVB = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
	};
}
