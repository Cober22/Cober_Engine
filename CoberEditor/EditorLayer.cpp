#include "EditorLayer.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	EditorLayer::EditorLayer()
		: Layer("Editor"), OrthoCamera({ 1280.0f, 720.0f }), PerspCamera(45.0f, { 1280.0f, 720.0f }, 0.1f, 500.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		// Create Lights
		// -----------
		// ----------- DIRECTIONAL Light
		dirLight = CreateRef<DirectionalLight>(
			glm::vec3(-0.2f, -1.0f, -0.3f),	// ---	Direction 
			glm::vec3(0.45f, 0.1f, 0.6f),	//----------------	Color
			0.1f, // -----------------------------  Ambient Intensity
			0.3f);// -----------------------------  Diffuse Intensity

		// ----------- POINT Lights
		for (unsigned int i = 0; i < std::size(pLightPos); i++) {
			Ref<PointLight> pointLight = CreateRef<PointLight>(
				pLightPos[i],	// -------	 Position
				Colors[i],		// -------	 Color
				0.5f, // -----------------   Ambient Intensity
				0.7f, // -----------------   Diffuse Intensity
				0.2f, // -----------------   Attenuation Linear
				0.1f);// -----------------   Attenuation Exponencial
			pointLights.push_back(pointLight);
		}
		// ----------- SPOT Lights
		Ref<SpotLight> spotLight = CreateRef<SpotLight>(
			PerspCamera.GetDirection(),	// Camera Direction 
			PerspCamera.GetPosition(),	// Camera Position
			glm::vec3(1.0f, 1.0f, 0.0f),// Color
			8.0f,	// -----------------   CutOff	
			10.0f,	// -----------------   OuterCutOff
			0.1f,	// -----------------   Ambient Intensity
			0.8f,	// -----------------   Diffuse Intensity
			0.09f,	// -----------------   Attenuation Linear
			0.032f);// -----------------   Attenuation Exponencial
		spotLights.push_back(spotLight);
		Ref<SpotLight> spotLight2 = CreateRef<SpotLight>(
			glm::vec3(-0.035, -0.95, -0.71),	// Camera Direction 
			glm::vec3(5.0f, 8.0f, 2.0f),	// Camera Position
			glm::vec3(1.0f, 0.0f, 0.0f),// Color
			15.0f,	// -----------------   CutOff	
			20.0f,	// -----------------   OuterCutOff
			0.4f,	// -----------------   Ambient Intensity
			1.0f,	// -----------------   Diffuse Intensity
			0.009f,	// -----------------   Attenuation Linear
			0.0032f);// ----------------   Attenuation Exponencial
		spotLights.push_back(spotLight2);

		// Create Textures
		// -----------
		steelBorderContainer =  Texture2D::Create("Assets/Textures/SteelBorderContainer.png");
		woodContainer = Texture2D::Create("Assets/Textures/GridObscure.png");
		//woodContainer =  Texture2D::Create("Assets/Textures/WoodenContainer.png");
		/*catTexture = Texture2D::Create("Assets/Textures/BlendTest.png");*/
		catTexture = Texture2D::Create("Assets/Textures/BlendTest.png");
		baseAtlas = Texture2D::Create("Assets/Textures/BaseAtlas.png");
		bridgeTexture = SubTexture2D::CreateFromCoords(baseAtlas, { 3, 4 }, { 128, 128 }, {1, 2});
		//checkerboardTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");

		// Load models
		// -----------
		gridModel = CreateRef<Mesh>();
		arenaModel = CreateRef<Mesh>();
		//
		gridModel->LoadMesh("Assets/Models/thegrid/GRID.obj");
		arenaModel->LoadMesh("Assets/Models/thegridFBX/GRID.fbx");

		// Framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{
		//m_Framebuffer->Bind();
	}

	void EditorLayer::OnUpdate( Timestep ts)
	{	
		CB_PROFILE_FUNCTION();

	
		// Camera Update
		{
			CB_PROFILE_SCOPE("CameraController::OnUpdate");
			if (perspective) {
				//if (m_ViewportFocused)
				PerspCamera.OnUpdate(ts);
			}
			else {
				//if (m_ViewportFocused)
				OrthoCamera.OnUpdate(ts);
			}
		}

		// Update
		{
			CB_PROFILE_SCOPE("Render Draw");
			m_Framebuffer->Bind();
			if (perspective)
				Renderer::BeginScene(PerspCamera);
			else
				Renderer::BeginScene(OrthoCamera);

			//m_Framebuffer->Bind();
			//CUBES!
			for (unsigned int i = 0; i < std::size(cubePositions); i++)
				Renderer::DrawCube(cubePositions[i], glm::vec3(1.0f), woodContainer, steelBorderContainer, { 1.0f, 1.0f, 1.0f });// cubeColors[color]);
		
			//LIGHTS!
			Renderer::DrawDirectionalLight(dirLight, true);
			Renderer::DrawPointLights(pointLights, true);
			//
			spotLights[0]->SetDirection(PerspCamera.GetDirection());
			spotLights[0]->SetPosition(PerspCamera.GetPosition());
			Renderer::DrawSpotLights(spotLights, true);
		
			//MODELS
			Renderer::DrawModel(gridModel, glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.5f));
			//Renderer::DrawModel(arenaModel);

			Renderer::DrawQuad({ 10.0, 10.0, -7.0f }, { 1.0f, 1.0f }, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
			Renderer::DrawRotatedQuad({ 10.0, 5.0, -7.0f}, 45.0f, { 1.0f, 1.0f });
			Renderer::DrawQuad({ 4.0, 5.0, -2.0f }, { 3.5f, 3.5f }, catTexture);
			Renderer::DrawRotatedQuad({ -4.0, 0.0, -15.0f }, 0.0f, { 1.0f, 2.0f }, bridgeTexture);
			Renderer::DrawRotatedQuad({ -10.0, 0.0, -2.0f }, 75.0f, { 2.0f, 2.0f }, catTexture);

			Renderer::EndScene();
			m_Framebuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		CB_PROFILE_FUNCTION();

		//if (ImGui::BeginMenuBar())
		//{
		//	if (ImGui::BeginMenu("File"))
		//	{
		//		// Disabling fullscreen would allow the window to be moved to the front of other windows, 
		//		// which we can't undo at the moment without finer window depth/z control.
		//		//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
		//
		//		if (ImGui::MenuItem("Exit")) Application::Get().Close();
		//		ImGui::EndMenu();
		//	}
		//
		//	ImGui::EndMenuBar();
		//}

		ImGui::Begin("Settings");
		//auto stats = Hazel::Renderer2D::GetStats();
		//ImGui::Text("Renderer2D Stats:");
		//ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		//ImGui::Text("Quads: %d", stats.QuadCount);
		//ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		//ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
			
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		////Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		//
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		
			PerspCamera.Resize(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::End();

		//ImGui::PopStyleVar();

	}

	void EditorLayer::OnEvent(SDL_Event& e)
	{
		//const Uint8* keystate = SDL_GetKeyboardState(NULL);
		//if (keystate[SDL_SCANCODE_0] && e.type == SDL_KEYDOWN)
		//	perspective = perspective == true ? false : true;
		//
		//if (perspective)
		//	PerspCamera.OnEvent(e);
		//else
		//	OrthoCamera.OnEvent(e);
	}
}