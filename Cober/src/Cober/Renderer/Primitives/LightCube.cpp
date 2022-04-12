#include "pch.h"
#include "Cober/Renderer/API/RenderCommand.h"
#include "Cober/Renderer/Primitives/Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	LightCube::LightCube() {
		Init();
	}

	void LightCube::Draw(const glm::vec3& position, const glm::vec3& size, Ref<Shader> shader) {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		shader->SetMat4("u_Model", transform);

		VAO->Bind();
		RenderCommand::DrawIndexed(VAO);
	}

	void LightCube::Init() {
		VAO = VertexArray::Create();

		// [---------- VERTEX BUFFER ----------]
		VBO = VertexBuffer::Create(vertices, sizeof(vertices));
		VBO->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		VAO->AddVertexBuffer(VBO);

		// [---------- INDEX BUFFER ----------]
		IBO = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		VAO->SetIndexBuffer(IBO);
		WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexturerData = 0xffffffff;
		WhiteTexture->SetData(&whiteTexturerData, sizeof(uint32_t));
	}
}


