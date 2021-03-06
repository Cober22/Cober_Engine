#include "pch.h"
#include "Cober/Renderer/RenderCommand.h"
#include "Cober/Renderer/Primitives/Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	Cube::Cube() {
		Init();
		shader = CreateRef<Shader>("Assets/Shaders/Primitive.glsl");
	}

	void Cube::Draw(const glm::mat4& transform, Ref<Shader> customShader) {
		if (customShader)
			shader = customShader;

		shader->SetMat4("u_Model", transform);
		shader->SetMat3("u_Normal", glm::transpose(glm::inverse(transform)));

		VAO->Bind();
		RenderCommand::DrawIndexed(VAO);
	}

	void Cube::Init() {
		VAO = VertexArray::Create();

		// [---------- VERTEX BUFFER ----------]
		VBO = VertexBuffer::Create(vertices, sizeof(vertices));
		VBO->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
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


