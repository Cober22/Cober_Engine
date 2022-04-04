#pragma once

#include "Engine.h"

class Sandbox2D : public Cober::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;

	void OnUpdate(Cober::Timestep ts) override;
	void OnEvent(SDL_Event& e) override;
private:
	Cober::PerspectiveCamera PerspCamera;
	Cober::OrthographicCamera OrthoCamera;

	bool perspective = true;
	Cober::Ref<Cober::Texture2D> catTexture;
	Cober::Ref<Cober::Texture2D> checkerboardTexture;
	Cober::Ref<Cober::Texture2D> woodContainer;
	Cober::Ref<Cober::Texture2D> steelBorderContainer;
	//Cober::Ref<Cober::Shader> m_ShaderSquare;

		// TEST
	glm::vec3 cubePositions[22] = {
		glm::vec3(	1.0f,	30.0f,	-15.0f),	
		glm::vec3(	0.0f,	-2.0f,	-9.0f),
		glm::vec3(	2.0f,	-2.0f,	-9.3f),
		glm::vec3(	0.5f,	 0.0f,	-10.0f),
		glm::vec3(	0.75f,	 2.0f,	-9.5f),
		glm::vec3(	3.4f,	-2.0f,	-30.0f),
		glm::vec3( -2.0f,	-2.0f,	-30.5f),
		glm::vec3( -3.3f,	 0.0f,	-30.0f),
		glm::vec3(	3.0f,	 0.0f,	-30.2f),
		glm::vec3( -4.0f,	-2.0f,	-30.5f),
		glm::vec3( -60.0f,	-2.0f,	-40.0f),
		glm::vec3( -2.0f,	-2.0f,	-40.0f),
		glm::vec3(	0.0f,	-2.0f,	-40.0f),
		glm::vec3( -60.0f,	-0.0f,	-40.0f),
		glm::vec3( -2.0f,	-0.0f,	-40.0f),
		glm::vec3(	0.0f,	-0.0f,	-40.0f),
		glm::vec3( -60.0f,	 2.0f,	-40.0f),
		glm::vec3( -2.0f,	 2.0f,	-40.0f),
		glm::vec3(	0.0f,	 2.0f,	-40.0f),
		glm::vec3( -60.0f,	 4.0f,	-40.0f),
		glm::vec3( -2.0f,	 4.0f,	-40.0f),
		glm::vec3(	0.0f,	 4.0f,	-40.0f),
	};

	glm::vec4 cubeColors[14] = {
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.2f, 0.8f, 0.3f, 1.0f),
		glm::vec4(0.8f, 0.2f, 0.3f, 1.0f),
		glm::vec4(0.75f, 0.1f, 0.75f, 1.0f),
		glm::vec4(0.87f, 0.35f, 1.0f, 1.0f),	// glm::vec4(0.2f, 0.3f, 1.0f, 1.0f),
		glm::vec4(0.1f, 0.5f, 0.5f, 1.0f),
		glm::vec4(1.0f, 0.5f, 0.3f, 1.0f),
		glm::vec4(0.5f, 0.5f, 0.1f, 1.0f),
		glm::vec4(0.3f, 0.7f, 0.6f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.8f, 0.6f, 0.1f, 1.0f),
	};

	glm::vec3 pointLightPositions[4] = {
		glm::vec3(	10.7f,	0.2f,	-2.0f),
		glm::vec3(	2.3f,	3.3f,	-44.0f),
		glm::vec3( -4.0f,	2.0f,	-12.0f),
		glm::vec3(	0.0f,	0.0f,	-30.0f),
	};

	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};
