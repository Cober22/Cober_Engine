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
	//Cober::OrthographicCameraController m_CameraController;
	Cober::PerspectiveCamera m_Camera;
	// Temp
	Cober::Ref<Cober::VertexArray> m_SquareVAO;
	Cober::Ref<Cober::Shader> m_ShaderSquare;

	Cober::Ref<Cober::Texture2D> m_TextureTest;

	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	// TEST
	bool m_Rotation;
	glm::vec3 m_CameraPosition = { 0, 0.0, 0.0 };
	float m_CameraRotation = 0.0f;
	float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 20.0f;
};
