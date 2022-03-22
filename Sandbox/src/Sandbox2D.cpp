#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	Cober::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f, }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Cober::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f, }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Cober::Renderer2D::EndScene();
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
