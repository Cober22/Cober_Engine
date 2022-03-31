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
	Cober::Ref<Cober::Texture2D> m_TextureTest;

	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};
