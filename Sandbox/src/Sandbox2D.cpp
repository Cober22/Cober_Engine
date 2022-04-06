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
	catTexture = Cober::Texture2D::Create("Assets/Textures/BlendTest.png");
	checkerboardTexture = Cober::Texture2D::Create("Assets/Textures/Checkerboard.png");
	
	woodContainer = Cober::Texture2D::Create("Assets/Textures/GridGreyDark.png");
	/*woodContainer = Cober::Texture2D::Create("Assets/Textures/WoodenContainer.png");*/
	steelBorderContainer = Cober::Texture2D::Create("Assets/Textures/SteelBorderContainer.png");

	// load models
	// -----------
	gridModel = Cober::CreateRef<Cober::Mesh>();
	gridModel->LoadMesh("Assets/Models/backpack/backpack.obj");
	//gridModel->LoadMesh("Assets/Models/chessBoard/Chess.fbx");
	//gridModel->LoadMesh("Assets/Models/thegrid/GRID.obj");
	//gridModel->LoadMesh("Assets/Models/wallWithGates/MuroGrades.fbx");
	//gridModel->LoadMesh("Assets/Models/test/untitled.obj");
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
		//Cober::RenderCommand::SetClearColor({ 0.02f, 0.008f, 0.05f, 1.0f });	// DARK BLUE
		//Cober::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Cober::RenderCommand::SetClearColor({ 1.0f, 0.6f, 0.3f, 1.0f });	// ORANGE
		//Cober::RenderCommand::SetClearColor({ 0.8f, 0.35f, 0.35f, 1.0f });
		Cober::RenderCommand::Clear();
	}
	
	{
		CB_PROFILE_SCOPE("Render Draw");
		if (perspective)
			Cober::Renderer::BeginScene(PerspCamera);
		else
			Cober::Renderer::BeginScene(OrthoCamera);

		int color = 0;
		for (unsigned int i = 0; i < std::size(cubePositions); i++) {
			if (i < std::size(pointLightPositions))
				Cober::Renderer::DrawLightCube(pointLightPositions[i], glm::vec3(0.4f), cubeColors[i]);
			if (i == 0)
				Cober::Renderer::DrawLightCube(cubePositions[i], glm::vec3(0.8f), { 1.0f, 1.0f, 1.0f, 1.0f });
			else
				Cober::Renderer::DrawCube(cubePositions[i], glm::vec3(1.0f), woodContainer, steelBorderContainer, { 1.0f, 1.0f, 1.0f, 1.0f });// cubeColors[color]);

			//if (++color >= std::size(cubeColors))
				//color = 0;
		}
		Cober::Renderer::DrawSquare({ 15.0, 0.0, -7.0f}, { 7.0f, 7.0f }, catTexture);
		Cober::Renderer::DrawModel(gridModel, glm::vec3(0.0f), glm::vec3(0.5f));

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
