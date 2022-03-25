#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_PerspCameraController(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f), 
						  m_OrthoCameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_TextureTest = Cober::Texture2D::Create("Assets/Textures/BlendTest.jpg");
	//m_OrthoCameraController(float left, float right, float bottom, float top);
	//m_PerspCameraController(glm::radians angle, float aspectRatio, floar nearPlane, float farPlane)
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Cober::Timestep ts)
{	
	CB_PROFILE_FUNCTION();

	// Update
	{
		CB_PROFILE_SCOPE("CameraController::OnUpdate");
		m_PerspCameraController.OnUpdate(ts);
	}

	// Render
	{
		CB_PROFILE_SCOPE("Render Prep");
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.6f, 0.3f, 1.0f });
		//Cober::RenderCommand::SetClearColor({ 0.8f, 0.35f, 0.35f, 1.0f });
		Cober::RenderCommand::Clear();
	}
	
	{
		CB_PROFILE_SCOPE("Render Draw");
		Cober::Renderer::BeginScene(m_PerspCameraController.GetCamera());
		Cober::Renderer::DrawQuad({ -1.0f, 0.5f, -15.0f }, { 0.8f, 0.8f }, { 0.2f, 0.8f, 0.3f, 1.0f });
		Cober::Renderer::DrawQuad({ 0.0f, 0.0f, -10.0f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//Cober::Renderer::DrawQuad({ 0.0f, 0.0f }, { 6.0f, 6.0f }, m_TextureTest);
		Cober::Renderer::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	//CB_PROFILE_FUNCTION();
	//ImGui::Begin("Settings");
	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	//ImGui::End();
}

void Sandbox2D::OnEvent(SDL_Event& e)
{
	//m_CameraController.OnEvent(e);
}
