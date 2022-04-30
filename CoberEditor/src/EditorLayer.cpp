#include "EditorLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Cober/Scene/SceneSerializer.h"

#include "ImGuizmo/ImGuizmo.h"

namespace Cober {

	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("Editor"), OrthoCamera({ 1280.0f, 720.0f }), PerspCamera(45.0f, { 1280.0f, 720.0f }, 0.1f, 500.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		CB_PROFILE_FUNCTION();
		// Create Lights
		// -----------
		// ----------- DIRECTIONAL Light
		dirLight = CreateRef<DirectionalLight>(
			glm::vec3(-0.2f, -1.0f, -0.3f),	// ---	Direction 
			glm::vec3(0.45f, 0.1f, 0.6f),	//----------------	Color
			//glm::vec3(1.0f, 1.0f, 1.0f),	//----------------	Color
			0.25f, // -----------------------------  Ambient Intensity
			0.3f); // -----------------------------  Diffuse Intensity

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
		// FlashLight
		//Ref<SpotLight> flashLight = CreateRef<SpotLight>(
		//	PerspCamera.GetDirection(),	// Camera Direction 
		//	PerspCamera.GetPosition(),	// Camera Position
		//	glm::vec3(1.0f, 1.0f, 0.0f),// Color
		//	8.0f,	// -----------------   CutOff	
		//	10.0f,	// -----------------   OuterCutOff
		//	0.1f,	// -----------------   Ambient Intensity
		//	0.8f,	// -----------------   Diffuse Intensity
		//	0.09f,	// -----------------   Attenuation Linear
		//	0.032f);// -----------------   Attenuation Exponencial
		//spotLights.push_back(flashLight);
		Ref<SpotLight> spotLight = CreateRef<SpotLight>(
			glm::vec3(-0.035, -0.95, -0.71),// Camera Direction 
			glm::vec3(5.0f, 8.0f, 2.0f),// Camera Position
			glm::vec3(1.0f, 0.0f, 0.0f),// Color
			15.0f,	// -----------------   CutOff	
			20.0f,	// -----------------   OuterCutOff
			0.4f,	// -----------------   Ambient Intensity
			1.0f,	// -----------------   Diffuse Intensity
			0.009f,	// -----------------   Attenuation Linear
			0.0032f);// ----------------   Attenuation Exponencial
		spotLights.push_back(spotLight);

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
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		// Scene
		m_ActiveScene = CreateRef<Scene>();
		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		m_IconPlay = Texture2D::Create("Assets/Icons/IconPlay.png");
		m_IconStop = Texture2D::Create("Assets/Icons/IconStop.png");


#if 0
		// Entity
		auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
		greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		m_SquareEntity = greenSquare;
		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_FirstCamera = m_ActiveScene->CreateEntity("Camera Perspective Entity");
		m_FirstCamera.AddComponent<CameraComponent>();
		m_SecondCamera = m_ActiveScene->CreateEntity("Camera Orthographic Entity");
		m_SecondCamera.AddComponent<CameraComponent>();
		m_SecondCamera.GetComponent<CameraComponent>().Primary = false;

		// Native Scripting Example
		class CameraController : public ScriptableEntity {
		public:
			virtual void OnCreate() override {
				// Test if OnCreate works well with different cameras 
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}
			virtual void OnDestroy() override {

			}
			virtual void OnUpdate(Timestep ts) override {

				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;
					if (Input::IsKeyPressed(KeyCode::A))
						translation.x -= speed * ts;
					if (Input::IsKeyPressed(KeyCode::D))
						translation.x += speed * ts;
					if (Input::IsKeyPressed(KeyCode::W))
						translation.y += speed * ts;
					if (Input::IsKeyPressed(KeyCode::S))
						translation.y -= speed * ts;
			}
		};

		m_FirstCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize("Assets/Scenes/ExampleScene.cober");
	}


	void EditorLayer::OnDetach()
	{
		CB_PROFILE_FUNCTION();
		//m_Framebuffer->Bind();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{	
		CB_PROFILE_FUNCTION();
		frames = ts.GetFrames();

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			//PerspCamera.Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Camera Update
		{
			CB_PROFILE_SCOPE("Render Draw");
			Renderer::ResetStats();
			m_Framebuffer->Bind();

			//RenderCommand::SetClearColor({ 0.02f, 0.008f, 0.05f, 1.0f });	// DARK BLUE
			//RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::SetClearColor({ 1.0f, 0.6f, 0.3f, 1.0f });	// ORANGE
			//RenderCommand::SetClearColor({ 0.8f, 0.35f, 0.35f, 1.0f });
			RenderCommand::Clear();
			// Clear our entity ID attachment to -1
			m_Framebuffer->ClearAttachment(1, -1);

			// Update Scene
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			switch (m_SceneState)
			{
				case GameState::EDIT:
				{
					//if (m_ViewportFocused)
						//m_CameraController.OnUpdate(ts);
					m_EditorCamera.OnUpdate(ts);
					m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
					break;
				}
				case GameState::PLAY:
				{
					m_ActiveScene->OnUpdateRuntime(ts);
					break;
				}
			}
			
			// CUBES!	// MOVE TO ENTITIES
			for (unsigned int i = 0; i < std::size(cubePositions); i++)
				Renderer::DrawCube(cubePositions[i], glm::vec3(1.0f), woodContainer, steelBorderContainer, { 1.0f, 1.0f, 1.0f });// cubeColors[color]);
		
			// LIGHTS!	// MOVE TO ENTITIES
			Renderer::DrawDirectionalLight(dirLight, true);
			Renderer::DrawPointLights(pointLights, true);
			// FlashLight
			//spotLights[0]->SetDirection(PerspCamera.GetDirection());
			//spotLights[0]->SetPosition(PerspCamera.GetPosition());
			Renderer::DrawSpotLights(spotLights, true);
		
			// MODELS	// MOVE TO ENTITIES
			Renderer::DrawModel(gridModel, glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.5f));
			// Renderer::DrawModel(arenaModel);

			// QUADS	// MOVE TO ENTITIES
			Renderer::DrawRotatedQuad({ 10.0, 5.0, -7.0f}, 45.0f, { 1.0f, 1.0f });
			Renderer::DrawQuad({ 4.0, 5.0, -2.0f }, { 3.5f, 3.5f }, catTexture);
			Renderer::DrawRotatedQuad({ -4.0, 0.0, -15.0f }, 0.0f, { 1.0f, 2.0f }, bridgeTexture);
			Renderer::DrawRotatedQuad({ -10.0, 0.0, -2.0f }, 75.0f, { 2.0f, 2.0f }, catTexture);
			Renderer::DrawQuad({ 10.0, 10.0, -7.0f }, { 1.0f, 1.0f }, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

			Renderer::EndScene();	// Delete when all geometry are entities

			auto[mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			glm::vec2 viewportSize= m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;
			
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
			}
			m_Framebuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		CB_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Load...", "Ctrl+O"))
					OpenFile();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		mFileDialog.Display();
		if (mFileDialog.HasSelected()) {
			auto file_path = mFileDialog.GetSelected().string();
			mFilePath = file_path;
			//mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);
			mFileSelected = true;
			mFileDialog.ClearSelected();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Settings");
		
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer::GetStats();
		ImGui::Text("Renderer Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Cubes: %d", stats.CubeCount);
		ImGui::Text("LightPoint: %d", stats.LightCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Frames: %d", frames);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		if (!ImGui::IsAnyItemActive())
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		else
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Select the index of the framebuffer pass
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenFile(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		if (selectedEntity && m_GizmoType != -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, 
							  m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection =  camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();;
			
			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;	// Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
								 (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
								 nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {

				glm::vec3 translation, rotation, scale;
				DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();
	
		ImGui::End();
	}

	void EditorLayer::UI_Toolbar() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == GameState::EDIT ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == GameState::EDIT)
				OnScenePlay();
			else if (m_SceneState == GameState::PLAY)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		PerspCamera.OnEvent(event);
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(CB_BIND_EVENT(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CB_BIND_EVENT(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {

		if (event.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		
		switch (event.GetKeyCode()) {
			case Key::N:
				if (control)
					NewScene();
				break;
			case Key::O:
				if (control)
					OpenFile();
				break;
			case Key::S:
				if (control && shift)
					SaveSceneAs();
				else if (control)
					SaveScene();
				break;

			// Gizmos
			case Key::Q: 
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;			  
			case Key::W: 
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;			  
			case Key::E: 
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;			  
			case Key::R: 
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {

		if (event.GetMouseButton() == Mouse::ButtonLeft)
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))	// Mouse picking
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		
		return false;
	}

	void EditorLayer::NewScene() {
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OpenFile() {
		mFileDialog.Open();
		if (mFileSelected)
			OpenFile(mFilePath);
	}
	void EditorLayer::OpenFile(const std::filesystem::path& path) {
		NewScene();
		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string());
		mFileSelected = false;
	}
	void EditorLayer::SaveSceneAs() {
		mFileDialog.Open();
		if (mFileSelected) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(mFilePath);
			mFileSelected = false;
		}
	}
	void EditorLayer::SaveScene() {
		if (!mFilePath.empty()) {
			std::string extension = mFilePath.substr(mFilePath.find_last_of("."));
			if (extension == ".cober") {
				SceneSerializer serializer(m_ActiveScene);
				serializer.Serialize(mFilePath);
			}
		}
	}

	void EditorLayer::OnScenePlay()	{
		m_SceneState = GameState::PLAY;
	}
	void EditorLayer::OnSceneStop() {
		m_SceneState = GameState::EDIT;
	}
}