#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platforms/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Cober::Timestep ts)
{	
	m_CameraController.OnUpdate(ts);
	
	Cober::RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
	Cober::RenderCommand::Clear();

	Cober::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Cober::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f, }, {0.8f, 0.2f, 0.3f, 1.0f});
	Cober::Renderer2D::EndScene();

	//Cober::Renderer::Submit(m_ShaderSquare, m_SquareVAO, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	//std::dynamic_pointer_cast<Cober::OpenGLShader>(m_ShaderSquare)->Bind();
	//std::dynamic_pointer_cast<Cober::OpenGLShader>(m_ShaderSquare)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(SDL_Event& e)
{
	m_CameraController.OnEvent(e);
}
