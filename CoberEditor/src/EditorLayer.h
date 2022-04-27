#pragma once

#include <Engine.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Cober/Scene/SceneSerializer.h"
#include "Cober/Renderer/Camera/EditorCamera.h"

using namespace glm;

namespace Cober {

	#define DIR_LIGHT		Ref<DirectionalLight>
	#define POINT_LIGHT		std::vector<Ref<PointLight>>
	#define SPOT_LIGHT		std::vector<Ref<SpotLight>>

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

		void NewScene();
		void OpenFile();
		void SaveSceneAs();
		void SaveScene();
	private:
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		EditorCamera m_EditorCamera;
		int m_GizmoType = -1;
	private:
		// create a file browser instance
		ImGui::FileBrowser mFileDialog;
		std::string mFilePath;
		bool mFileSelected = false;
	private:
		PerspectiveCamera PerspCamera;
		OrthographicCamera OrthoCamera;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;
		uint32_t frames;

		// Entities
		Entity m_SquareEntity;
		Entity m_FirstCamera;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;
		// Textures
		Ref<Texture2D> catTexture;
		Ref<Texture2D> checkerboardTexture;
		Ref<Texture2D> woodContainer;
		Ref<Texture2D> steelBorderContainer;
		Ref<Texture2D> baseAtlas;
		Ref<SubTexture2D> bridgeTexture;
		// Models
		Ref<Mesh> gridModel;
		Ref<Mesh> arenaModel;
		// Lights
		DIR_LIGHT	dirLight;
		POINT_LIGHT	pointLights;
		SPOT_LIGHT	spotLights;
		//Cober::Ref<Cober::Shader> m_ShaderQuad;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };

		struct ProfileResult 
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;


		vec3 pLightPos[4] = {
			vec3(  -7.0f,	0.2f,	-2.0f),
			vec3(	2.3f,	3.3f,	-44.0f),
			vec3(  -4.0f,	2.0f,	-12.0f),
			vec3(	0.0f,	0.0f,	-30.0f),
		};

		vec3 Colors[4] = {
			vec3(0.0f, 1.0f, 0.0f),
			vec3(0.0f, 0.0f, 1.0f),
			vec3(1.0f, 1.0f, 0.0f),
			vec3(1.0f, 0.0f, 1.0f),
			//(0.2f, 0.8f, 0.3f, 1.0f),
			//(0.8f, 0.2f, 0.3f, 1.0f),
			//(0.75f, 0.1f, 0.75f, 1.0f),
			//(0.87f, 0.35f, 1.0f, 1.0f),	// glm::vec4(0.2f, 0.3f, 1.0f, 1.0f),
			//(0.1f, 0.5f, 0.5f, 1.0f),
			//(1.0f, 0.5f, 0.3f, 1.0f),
			//(0.5f, 0.5f, 0.1f, 1.0f),
			//(0.3f, 0.7f, 0.6f, 1.0f),
			//(1.0f, 1.0f, 1.0f, 1.0f),
			//(0.8f, 0.6f, 0.1f, 1.0f),
		};

		vec3 cubePositions[21] = {
			//vec3(1.0f,	30.0f,	-15.0f),
			vec3(0.0f,	-2.0f,	-9.0f),
			vec3(2.0f,	-2.0f,	-9.3f),
			vec3(0.5f,	 0.0f,	-10.0f),
			vec3(0.75f,	 2.0f,	-9.5f),
			vec3(3.4f,	-2.0f,	-30.0f),
			vec3(-2.0f,	-2.0f,	-30.5f),
			vec3(-3.3f,	 0.0f,	-30.0f),
			vec3(3.0f,	 0.0f,	-30.2f),
			vec3(-4.0f,	-2.0f,	-30.5f),
			vec3(-60.0f,	-2.0f,	-40.0f),
			vec3(-2.0f,	-2.0f,	-40.0f),
			vec3(0.0f,	-2.0f,	-40.0f),
			vec3(-60.0f,	-0.0f,	-40.0f),
			vec3(-2.0f,	-0.0f,	-40.0f),
			vec3(0.0f,	-0.0f,	-40.0f),
			vec3(-60.0f,	 2.0f,	-40.0f),
			vec3(-2.0f,	 2.0f,	-40.0f),
			vec3(0.0f,	 2.0f,	-40.0f),
			vec3(-60.0f,	 4.0f,	-40.0f),
			vec3(-2.0f,	 4.0f,	-40.0f),
			vec3(0.0f,	 4.0f,	-40.0f),
		};
	};
}
