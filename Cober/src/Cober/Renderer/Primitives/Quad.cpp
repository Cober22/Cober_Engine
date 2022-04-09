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
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			//{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float,  "a_TexIndex" },
			{ ShaderDataType::Float,  "a_TilingFactor" }
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

		vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Quad::Draw(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color) {
		
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { 
						{ 0.0f, 0.0f }, 
						{ 1.0f, 0.0f }, 
						{ 1.0f, 1.0f }, 
						{ 0.0f, 1.0f } 
					};
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < vertexCount; i++) {
			attributes->Position = transform * vertexPositions[i];
			attributes->Color = color;
			attributes->TexCoord = textureCoords[i];
			attributes->TexIndex = textureIndex;
			attributes->TilingFactor = tilingFactor;
			attributes++;
		}
		indexCount += 6;
		//std::cout << "Color: " << attributes->Color.x << " " << attributes->Color.y << " " << attributes->Color.z << std::endl;
		//std::cout << "TexCoord: " << attributes->TexCoord.x << " " << attributes->TexCoord.y << std::endl;
		//std::cout << "TexIndex: " << attributes->TexIndex << std::endl;
		//std::cout << "TilingFactor: " << attributes->TilingFactor << std::endl;
	}
	
	void Quad::Draw(const glm::vec3& position, float rotation, const glm::vec2& size, 
					const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {

		//constexpr size_t quadVertexCount = 4;
		//constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		// For loading Sprite Sheets...
		// ............................
		constexpr float x = 3, y = 3;	// Index of the sprite to load	// Provisional
		constexpr float sheetWidth = 1024.0f, sheetHeight = 1024.0f;	// Provisional
		constexpr float spriteWidth = 128.0f, spriteHeight = 128.0f;	// Provisional
		//constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = {
			{(x * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight },
			{(x + 1) * spriteWidth / sheetWidth, (y * spriteHeight) / sheetHeight},
			{(x + 1) * spriteWidth / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight},
			{(x * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight}
		};

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < textureSlotIndex; i++) {

			if (textureSlots[i].get() == texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {

			if (indexCount >= maxIndices)
				Renderer::FlushAndReset();

			textureIndex = (float)textureSlotIndex;
			textureSlots[textureSlotIndex] = texture;
			textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < vertexCount; i++)
		{
			attributes->Position = transform * vertexPositions[i];
			attributes->Color = color;
			attributes->TexCoord = textureCoords[i];
			attributes->TexIndex = textureIndex;
			attributes->TilingFactor = tilingFactor;
			attributes++;
		}
		indexCount += 6;
	}
}