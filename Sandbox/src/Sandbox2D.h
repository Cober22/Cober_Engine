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
	Cober::OrthographicCameraController m_CameraController;
	
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
};
