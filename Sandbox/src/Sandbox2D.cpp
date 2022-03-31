#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), OrthoCamera({ 1280.0f, 720.0f }), PerspCamera(45.0f, { 1280.0f, 720.0f }, 0.1f, 100.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_TextureTest = Cober::Texture2D::Create("Assets/Textures/BlendTest.png");
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
		if (perspective)
			PerspCamera.OnUpdate(ts);
		else
			OrthoCamera.OnUpdate(ts);
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
		if (perspective)
			Cober::Renderer::BeginScene(PerspCamera);
		else
			Cober::Renderer::BeginScene(OrthoCamera);

	
		// TEST
		glm::vec3 cubePositions[10] = {
			glm::vec3(0.0f,  3.5f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		glm::vec4 cubeColors[10] = {
			glm::vec4(0.2f, 0.8f, 0.3f, 1.0f),
			glm::vec4(0.2f, 0.3f, 0.8f, 1.0f),
			glm::vec4(0.8f, 0.2f, 0.3f, 1.0f),
			glm::vec4(0.5f, 0.5f, 0.1f, 1.0f),
			glm::vec4(0.1f, 0.5f, 0.5f, 1.0f),
			glm::vec4(0.5f, 0.1f, 0.5f, 1.0f),
			glm::vec4(0.8f, 0.6f, 0.1f, 1.0f),
			glm::vec4(0.7f, 0.1f, 0.4f, 1.0f),
			glm::vec4(0.5f, 0.2f, 0.7f, 1.0f),
			glm::vec4(0.3f, 0.7f, 0.6f, 1.0f)
		};

		for (unsigned int i = 0; i < 10; i++) {
			Cober::Renderer::DrawCube(cubePositions[i], { 0.8f, 0.8f }, cubeColors[i]);
		}

		//Cober::Renderer::DrawQuad({ -1.0f, 0.5f, -15.0f }, { 0.8f, 0.8f }, { 0.2f, 0.8f, 0.3f, 1.0f });
		//Cober::Renderer::DrawQuad({ 0.0f, 0.0f, -10.0f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Cober::Renderer::DrawSquare({ 0.0f, 0.0f }, { 6.0f, 6.0f }, m_TextureTest);
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
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_0] && e.type == SDL_KEYDOWN)
		perspective = perspective == true ? false : true;

	if (perspective)
		PerspCamera.OnEvent(e);
	else
		OrthoCamera.OnEvent(e);
}
