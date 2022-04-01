#include "pch.h"
#include "Cober/Renderer/API/RenderCommand.h"
#include "Cober/Renderer/Primitives/Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	Square::Square() {
		Init();
	}

	void Square::Draw(const glm::vec3& position, const glm::vec2& size, Ref<Shader> shader) {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		shader->SetMat4("u_Transform", transform);

		VAO->Bind();
		RenderCommand::DrawIndexed(VAO);
	}

	void Square::Init() {
		VAO = VertexArray::Create();

		// [---------- VERTEX BUFFER ----------]
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TextCoord" }
			});
		VAO->AddVertexBuffer(squareVB);

		// [---------- INDEX BUFFER ----------]
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		VAO->SetIndexBuffer(squareIB);
		WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexturerData = 0xffffffff;
		WhiteTexture->SetData(&whiteTexturerData, sizeof(uint32_t));
	}
}