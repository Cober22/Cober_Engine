#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_Camera(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f)//m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_TextureTest = Cober::Texture2D::Create("Assets/Textures/BlendTest.png");
	//m_CameraController.SetProjection();	
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
		//m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		CB_PROFILE_SCOPE("Render Prep");
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.5f, 0.2f, 1.0f });
		Cober::RenderCommand::Clear();
	}
	
	{
		CB_PROFILE_SCOPE("Render Draw");

		//TEST
		
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		// --- Keyboard keys
		if (keystate[SDL_SCANCODE_A]) {
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_D]) {
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_S]) {
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_W]) {
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_Q]) {
			m_CameraRotation += -sin(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
			m_CameraRotation += cos(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_E]) {
			m_CameraRotation -= -sin(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
			m_CameraRotation -= cos(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
		}
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		//Cober::Renderer2D::DrawQuad(m_CameraPosition, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		// END TEST
		
		Cober::Renderer2D::BeginScene(m_Camera);// .GetCamera());
		//Cober::Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 0.8f, 0.8f }, { 0.2f, 0.8f, 0.3f, 1.0f });
		Cober::Renderer2D::DrawQuad({ 0.0f, 0.0f, -10.0f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//Cober::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 6.0f, 6.0f }, m_TextureTest);
		Cober::Renderer2D::EndScene();
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
