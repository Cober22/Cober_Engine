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
			m_Framebuffer->Bind();
			// BACKGRUOND COLOR!
			//RenderCommand::SetClearColor({ 0.02f, 0.008f, 0.05f, 1.0f });	// DARK BLUE
			//RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			 RenderCommand::SetClearColor({ 1.0f, 0.6f, 0.3f, 1.0f });	// ORANGE
			//RenderCommand::SetClearColor({ 0.8f, 0.35f, 0.35f, 1.0f });
			 RenderCommand::Clear();
		}
	
		{
			CB_PROFILE_SCOPE("Render Draw");
			if (perspective)
				Renderer::BeginScene(PerspCamera);
			else
				Renderer::BeginScene(OrthoCamera);

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
	
		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");

			//auto stats = Renderer::GetStats();
			//ImGui::Text("Renderer2D Stats:");
			//ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			//ImGui::Text("Quads: %d", stats.QuadCount);
			//ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			//ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 1080, 640 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::End();

			ImGui::End();
		}
		else
		{
			ImGui::Begin("Settings");

			//auto stats = Renderer::GetStats();
			//ImGui::Text("Renderer2D Stats:");
			//ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			//ImGui::Text("Quads: %d", stats.QuadCount);
			//ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			//ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

			uint32_t textureID = catTexture->GetRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(SDL_Event& e)
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_0] && e.type == SDL_KEYDOWN)
			perspective = perspective == true ? false : true;
	
		if (perspective)
			PerspCamera.OnEvent(e);
		else
			OrthoCamera.OnEvent(e);
	}
}