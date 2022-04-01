#include "pch.h"
#include "Cober/Renderer/API/RenderCommand.h"
#include "Cober/Renderer/Primitives/Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	Cube::Cube() {
		Init();
	}

	void Cube::Draw(const glm::vec3& position, const glm::vec3& size, Ref<Shader> shader) {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		shader->SetMat4("u_Transform", transform);

		VAO->Bind();
		RenderCommand::DrawIndexed(VAO);
	}

	void Cube::Init() {
		VAO = VertexArray::Create();

		// [---------- VERTEX BUFFER ----------]
		cubeVB = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TextCoord" }
			});
		VAO->AddVertexBuffer(cubeVB);

		// [---------- INDEX BUFFER ----------]
		cubeIB = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
		VAO->SetIndexBuffer(cubeIB);
		WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexturerData = 0xffffffff;
		WhiteTexture->SetData(&whiteTexturerData, sizeof(uint32_t));
	}
}


