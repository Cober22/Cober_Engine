#include "pch.h"
#include "Cober/Renderer/API/RenderCommand.h"
#include "Cober/Renderer/Primitives/Primitives.h"
#include "Cober/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	Quad::Quad() {
		Init();
	}

	void Quad::Init() {

		//stats = CreateRef<Stats>();
		VAO = VertexArray::Create();

		// [---------- VERTEX BUFFER ----------]
		VBO = VertexBuffer::Create(maxVertices * sizeof(Attributes));
		VBO->SetLayout({
			{ ShaderDataType::Float3, "a_Position"		},
			{ ShaderDataType::Float4, "a_Color"			},
			{ ShaderDataType::Float2, "a_TexCoord"		},
			{ ShaderDataType::Float,  "a_TexIndex"		},
			{ ShaderDataType::Float,  "a_TilingFactor"	},
			{ ShaderDataType::Float3, "a_Normal"		},
			{ ShaderDataType::Int,	  "a_EntityID"		}
			});
		VAO->AddVertexBuffer(VBO);

		// [---------- INDEX BUFFER ----------]
		uint32_t* quadIndices = new uint32_t[maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, maxIndices);
		VAO->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[maxTextureSlots];
		for (uint32_t i = 0; i < maxTextureSlots; i++)
			samplers[i] = i;

		shader = Shader::Create("Assets/Shaders/Texture.glsl");
		shader->Bind();
		shader->SetIntArray("u_Textures", samplers, maxTextureSlots);

		textureSlots[0] = WhiteTexture;

		vertexPositions[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
		vertexPositions[1] = {  1.0f, -1.0f, 0.0f, 1.0f };
		vertexPositions[2] = {  1.0f,  1.0f, 0.0f, 1.0f };
		vertexPositions[3] = { -1.0f,  1.0f, 0.0f, 1.0f };
	}
	void Quad::Draw(const glm::mat4& transform, const glm::vec4& color, float tilingFactor, int entityID) {
		
		const int textureIndex = 0; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (indexCount >= maxIndices)
			Renderer::FlushAndReset();

		SetAttributes(transform, color, textureIndex, textureCoords, tilingFactor, entityID);
	}
	
	void Quad::Draw(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor, int entityID) {

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (indexCount >= maxIndices)
			Renderer::FlushAndReset();

		int textureIndex = 0;
		for (uint32_t i = 1; i < textureSlotIndex; i++) {

			if (textureSlots[i].get() == texture.get()) {
				textureIndex = (int)i;
				break;
			}
		}

		if (textureIndex == 0) {

			if (indexCount >= maxIndices)
				Renderer::FlushAndReset();

			textureIndex = textureSlotIndex;
			textureSlots[textureSlotIndex] = texture;
			textureSlotIndex++;
		}

		SetAttributes(transform, color, textureIndex, textureCoords, tilingFactor, entityID);
	}

	void Quad::Draw(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor, int entityID) {
		
		// For loading Sprite Sheets...
		// ............................
		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		if (indexCount >= maxIndices)
			Renderer::FlushAndReset();

		int textureIndex = 0;
		for (uint32_t i = 1; i < textureSlotIndex; i++) {

			if (textureSlots[i].get() == texture.get()) {
				textureIndex = (int)i;
				break;
			}
		}

		if (textureIndex == 0) {

			if (indexCount >= maxIndices)
				Renderer::FlushAndReset();

			textureIndex = textureSlotIndex;
			textureSlots[textureSlotIndex] = texture;
			textureSlotIndex++;
		}

		SetAttributes(transform, color, textureIndex, textureCoords, tilingFactor, entityID);
	}

	void Quad::Draw(const glm::mat4& transform, const glm::vec4& color, int entityID) {

		const int textureIndex = 0; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (indexCount >= maxIndices)
			Renderer::FlushAndReset();

		SetAttributes(transform, color, textureIndex, textureCoords, 1.0f, entityID);
	}

	void Quad::SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID) {

		for (size_t i = 0; i < vertexCount; i++) {
			attributes->Position = transform * vertexPositions[i];
			attributes->Color = color;
			attributes->TexCoord = textureCoords[i];
			attributes->TexIndex = textureIndex;
			attributes->TilingFactor = tilingFactor;
			attributes->Normal = glm::vec3(0.0f, 0.0f, 1.0f);
			attributes->EntityID = entityID;
			attributes++;
		}
		indexCount += 6;

		shader->SetMat4("u_Model", glm::mat4(1.0f));
		shader->SetMat3("u_Normal", glm::transpose(glm::inverse(glm::mat4(1.0f))));
	}
}