#pragma once

#include "Engine.h"

class Sandbox2D : public Cober::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Cober::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(SDL_Event& e) override;
private:
	Cober::OrthographicCameraController m_CameraController;
	
	// Temp
	Cober::Ref<Cober::VertexArray> m_SquareVAO;
	Cober::Ref<Cober::Shader> m_ShaderSquare;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
