#include "EditorLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Cober/Scene/SceneSerializer.h"

#include "ImGuizmo/ImGuizmo.h"

namespace Cober {

	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("Editor")
	{
	}

	void EditorLayer::OnAttach()
	{
		CB_PROFILE_FUNCTION();
		
		// Framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		// Scene
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		m_IconPlay = Texture2D::Create("Assets/Icons/IconPlay.png");
		m_IconStop = Texture2D::Create("Assets/Icons/IconStop.png");
		m_IconBackgroundSettings = Texture2D::Create("Assets/Icons/orangeMykoeski.png");

		// Audio
		AudioManager::SetupInstance();

		// SCRIPT TEST
#if 0
		// Native Scripting Example
		class CameraController : public ScriptableEntity {
		public:
			virtual void OnCreate() override {
				// Test if OnCreate works well with different cameras 
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;

				camera = Find("Camera");
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
						translation.z += speed * ts;
					if (Input::IsKeyPressed(KeyCode::S))
						translation.z -= speed * ts;
			}
		private:
			Entity camera;
		};

		//m_FirstCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		//m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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
			//auto& colors = ImGui::GetStyle().Colors;
			m_Framebuffer->Bind();

			RenderCommand::SetClearColor({ 0.05f, 0.03f, 0.08f, 1.0f });	// DARK BLUE
			//RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			//RenderCommand::SetClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
			//RenderCommand::SetClearColor({ 1.0f, 0.6f, 0.3f, 1.0f });	// ORANGE
			//RenderCommand::SetClearColor({ 0.8f, 0.35f, 0.35f, 1.0f });
			RenderCommand::Clear();

			// Clear our entity ID attachment to -1
			m_Framebuffer->ClearAttachment(1, -1);
			
			switch (m_SceneState)
			{
				case GameState::EDIT:
				{
					//colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
					m_EditorCamera.OnUpdate(ts);
					m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

					break;
				}
				case GameState::PLAY:
				{	
					//colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
					m_ActiveScene->OnUpdateRuntime(ts);
					break;
				}
			}

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

		//ImGui::ShowDemoWindow();

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
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::Checkbox("Guizmo Local", &guizmoLocal))
					guizmoMode = guizmoLocal ? ImGuizmo::LOCAL : ImGuizmo::WORLD;

				if (ImGui::Checkbox("3D World", &WorldType))
					m_EditorScene->SetWorldType(WorldType);
			
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		mFileDialog.Display();
		if (mFileDialog.HasSelected()) {
			auto file_path = mFileDialog.GetSelected().string();
			mFilePath = file_path;
			//mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);
			OpenFileDialog(mFilePath);
	
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
				mFilePath = (std::filesystem::path(g_AssetPath) / path).string();
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
								 (ImGuizmo::OPERATION)m_GizmoType, (ImGuizmo::MODE)guizmoMode, glm::value_ptr(transform),
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
		//PerspCamera.OnEvent(event);
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
		
		if (m_SceneState == GameState::EDIT)
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
				case Key::D:
					if (control)
						DuplicateSelectedEntity();
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

	void EditorLayer::OpenFileDialog(const std::filesystem::path& path) {
		if (mMenuFileOption == MenuOptions::OPEN)
			OpenFile(path);
		else if (mMenuFileOption == MenuOptions::SAVE_AS) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(path.string()); 
		}
	}
	void EditorLayer::OpenFile() {
		mMenuFileOption = MenuOptions::OPEN;
		mFileDialog.Open();
	}

	void EditorLayer::OpenFile(const std::filesystem::path& path) {
		if (m_SceneState != GameState::EDIT)
			OnSceneStop();

		if (path.extension().string() != ".cober")
			return;

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_ActiveScene = m_EditorScene;
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		}
	}
	
	void EditorLayer::SaveSceneAs() {
		mMenuFileOption = MenuOptions::SAVE_AS;
		mFileDialog.Open();
	}
	void EditorLayer::SaveScene() {
		if (!mFilePath.empty()) {
			std::string extension = mFilePath.substr(mFilePath.find_last_of("."));
			if (extension == ".cober") {
				SceneSerializer serializer(m_ActiveScene);
				serializer.Serialize(mFilePath);
			}
		}
		else
			SaveSceneAs();
	}

	void EditorLayer::OnScenePlay()	{
		m_SceneState = GameState::PLAY;
		// Make a copy of the Editor scene
		m_RuntimeScene = Scene::Copy(m_EditorScene);
		m_ActiveScene = m_RuntimeScene;
		m_ActiveScene->OnRuntimeStart();
	}
	void EditorLayer::OnSceneStop() {
		m_SceneState = GameState::EDIT;
		m_ActiveScene = m_EditorScene;
		m_RuntimeScene = nullptr;
		m_ActiveScene->OnRuntimeStop();
	}

	void EditorLayer::DuplicateSelectedEntity() {
		if (m_SceneState != GameState::EDIT)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}
}