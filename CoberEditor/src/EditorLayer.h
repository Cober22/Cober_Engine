#pragma once

#include <Engine.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Cober/Scene/SceneSerializer.h"
#include "Cober/Renderer/Camera/EditorCamera.h"

using namespace glm;

namespace Cober {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& event) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		void NewScene();
		void OpenFileDialog(const std::filesystem::path& path);
		void OpenFile();
		void OpenFile(const std::filesystem::path& path);
		void SaveSceneAs();
		void SaveScene();

		void OnScenePlay();
		void OnSceneStop();
		void DuplicateSelectedEntity();

		// UI Panels
		void UI_Toolbar();
	private:
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		EditorCamera m_EditorCamera;
		int m_GizmoType = -1;
		int guizmoMode;
		bool guizmoLocal;
		bool WorldType;
	private:
		// create a file browser instance
		ImGui::FileBrowser mFileDialog;
		std::string mFilePath;
		enum MenuOptions { OPEN = 0, SAVE_AS };
		MenuOptions mMenuFileOption;
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene, m_EditorScene, m_RuntimeScene;
		uint32_t frames;

		GameState m_SceneState = GameState::EDIT;
		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;
		Ref<Texture2D> m_IconBackgroundSettings;

		Entity m_HoveredEntity;
		
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_ViewportBounds[2];

		struct ProfileResult 
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;
	};
}
