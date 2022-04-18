#pragma once

#include <Engine.h>

using namespace Cober;
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
		virtual void OnImGuiRender() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent() override;
	private:
		PerspectiveCamera PerspCamera;
		OrthographicCamera OrthoCamera;

		Ref<Framebuffer> m_Framebuffer;

		bool perspective = true;
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
