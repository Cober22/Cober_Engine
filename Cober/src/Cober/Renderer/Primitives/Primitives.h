#pragma once
#include "Cober/Renderer/VertexArray.h"
#include "Cober/Renderer/Texture.h"
#include "Cober/Renderer/Shader.h"

namespace Cober {

	class Quad {
	public:
		Quad();
		void Quad::Draw(const glm::mat4& transform, Ref<Shader> shader, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		void Quad::Draw(const glm::mat4& transform, const Ref<Texture2D>& texture, Ref<Shader> shader, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		void Quad::Draw(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, Ref<Shader> shader, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		void Quad::Draw(const glm::mat4& transform, const glm::vec4& color, Ref<Shader> shader, int entityID);

		void Init();
		// Functions
		Ref<VertexBuffer> GetVBO() { return VBO; }
		Ref<VertexArray> GetVAO() { return VAO; }
		Ref<Texture> GetTexture() { return WhiteTexture; }
		Ref<Shader> GetShader() { return shader; }

		// Functions
		static const uint32_t maxQuads = 20000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;
		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;
		uint32_t indexCount = 0;	// 0 = white texture

		struct Attributes {
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;
			glm::vec3 Normal;

			// Editor-only
			int EntityID;
		};
		Attributes* attributes = nullptr;
	private:
		void SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID);

		// Buffer Objects
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;
		Ref<VertexArray> VAO;
		Ref<Shader> shader;
		Ref<Texture2D> WhiteTexture;

		glm::vec4 vertexPositions[4];
		size_t vertexCount = 4;
	};

	class Cube {
	public:
		Cube();
		void Draw(const glm::mat4& transform, Ref<Shader> customShader);
		void Init();
		Ref<VertexArray> GetVAO() { return VAO; }
		Ref<Texture> GetTexture() { return WhiteTexture; }
		Ref<Shader> GetShader() { return shader; }

	private:
		// Primitive Attributes
		Ref<VertexArray> VAO;
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> shader;

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
		void Draw(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color = glm::vec3(1.0f));
		void Init();
		Ref<VertexArray> GetVAO() { return VAO; }
		Ref<Texture> GetTexture() { return WhiteTexture; }
		Ref<Shader> GetShader() { return shader; }
		void SetShader(Ref<Shader> Shader) { shader = Shader; }

	private:
		// Primitive Attributes
		Ref<VertexArray> VAO;
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> shader;

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
