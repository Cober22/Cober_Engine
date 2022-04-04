#pragma once
#include "Cober/Renderer/VertexArray.h"
#include "Cober/Renderer/Texture.h"
#include "Cober/Renderer/Shader.h"

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
		float vertices[8 * 4] = {
			// positions			// normals			// texture coords
			-1.0,	-1.0,	 1.0,	0.0,	0.0,	1.0,	0.0,	0.0,
			 1.0,	-1.0,	 1.0,	0.0,	0.0,	1.0,	1.0,	0.0,
			 1.0,	 1.0,	 1.0,	0.0,	0.0,	1.0,	1.0,	1.0,
			-1.0,	 1.0,	 1.0,	0.0,	0.0,	1.0,	0.0,	1.0,
		};
		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		// Buffer Objects
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;
	};

	class Cube {
	public:
		Cube();
		void Draw(const glm::vec3& position, const glm::vec3& size, Ref<Shader> shader);
		void Init();
		Ref<VertexArray> GetVAO() { return VAO; }
		Ref<Texture> GetTexture() { return WhiteTexture; }

	private:
		// Primitive Attributes
		Ref<VertexArray> VAO;
		Ref<Texture2D> WhiteTexture;

		// Data		
		float vertices[6 * 4 * (3 + 3 + 2)] = {
			// positions			// normals				// texture coords
			// front
			-1.0,	-1.0,	 1.0,	0.0,	0.0,	1.0,	0.0,	0.0,
			 1.0,	-1.0,	 1.0,	0.0,	0.0,	1.0,	1.0,	0.0, 
			 1.0,	 1.0,	 1.0,	0.0,	0.0,	1.0,	1.0,	1.0,
			-1.0,	 1.0,	 1.0,	0.0,	0.0,	1.0,	0.0,	1.0,
			// top
			-1.0,	 1.0,	 1.0,	0.0,	1.0,	0.0,	0.0,	0.0,
			 1.0,	 1.0,	 1.0,	0.0,	1.0,	0.0,	1.0,	0.0,
			 1.0,	 1.0,	-1.0,	0.0,	1.0,	0.0,	1.0,	1.0,
			-1.0,	 1.0,	-1.0,	0.0,	1.0,	0.0,	0.0,	1.0,
			// back
			 1.0,	-1.0,	-1.0,	0.0,	0.0,   -1.0,	0.0,	0.0,
			-1.0,	-1.0,	-1.0,	0.0,	0.0,   -1.0,	1.0,	0.0,
			-1.0,	 1.0,	-1.0,	0.0,	0.0,   -1.0,	1.0,	1.0,
			 1.0,	 1.0,	-1.0,	0.0,	0.0,   -1.0,	0.0,	1.0,
			// left
			-1.0,	-1.0,	-1.0,  -1.0,	0.0,	0.0,	0.0,	0.0,
			-1.0,	-1.0,	 1.0,  -1.0,	0.0,	0.0,	1.0,	0.0,
			-1.0,	 1.0,	 1.0,  -1.0,	0.0,	0.0,	1.0,	1.0,
			-1.0,	 1.0,	-1.0,  -1.0,	0.0,	0.0,	0.0,	1.0,
			// right
			 1.0,	-1.0,	 1.0,   1.0,	0.0,	0.0,	0.0,	0.0,
			 1.0,	-1.0,	-1.0,   1.0,	0.0,	0.0,	1.0,	0.0,
			 1.0,	 1.0,	-1.0,   1.0,	0.0,	0.0,	1.0,	1.0,
			 1.0,	 1.0,	 1.0,   1.0,	0.0,	0.0,	0.0,	1.0,
			// bottom
			-1.0,	-1.0,	-1.0,	0.0,   -1.0,	0.0,	0.0,	0.0,
			 1.0,	-1.0,	-1.0,	0.0,   -1.0,	0.0,	1.0,	0.0,
			 1.0,	-1.0,	 1.0,	0.0,   -1.0,	0.0,	1.0,	1.0,
			-1.0,	-1.0,	 1.0,	0.0,   -1.0,	0.0,	0.0,	1.0,
		};
		
		uint32_t indices[6 * 2 * 3] = {
			// front
			0,   1,  2,
			2,   3,  0,
			// top
			4,   5,  6,
			6,   7,  4,
			// back
			8,   9, 10,
			10, 11,  8,
			// left
			16, 17, 18,
			18, 19, 16,
			// right
			20, 21, 22,
			22, 23, 20,
			// bottom
			12, 13, 14,
			14, 15, 12,
		};

		// Buffer Objects
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;
	};

	class LightCube {
	public:
		LightCube();
		void Draw(const glm::vec3& position, const glm::vec3& size, Ref<Shader> shader);
		void Init();
		Ref<VertexArray> GetVAO() { return VAO; }
		Ref<Texture> GetTexture() { return WhiteTexture; }

	private:
		// Primitive Attributes
		Ref<VertexArray> VAO;
		Ref<Texture2D> WhiteTexture;

		// Data
		float vertices[2 * 3 * 4] = {
			// front
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,
			 1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
			// back
			-1.0, -1.0,  1.0,
			 1.0, -1.0,  1.0,
			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
		};
		uint32_t indices[6 * 2 * 3] = {
			// front
			0, 1, 3,
			1, 2, 3,
			// back
			4, 5, 7,
			5, 6, 7,
			// right
			0, 1, 4,
			1, 4, 5,
			// left
			2, 3, 7,
			2, 6, 7,
			// top
			0, 3, 4,
			3, 4, 7,
			// bottom
			1, 2, 5,
			2, 5, 6,
		};

		// Buffer Objects
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;
	};
}