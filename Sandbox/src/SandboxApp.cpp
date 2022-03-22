#include <Engine.h>
#include <Cober/EntryPoint.h>

#include "ImGui/imgui.h"

#include "Sandbox2D.h"

class ExampleLayer : public Cober::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate(Cober::Timestep ts) override
	{

	}

	void OnImGuiRender() override 
	{

	}

	void OnEvent(SDL_Event& event) override
	{
	}

private:
};

class Sandbox : public Cober::Application {

public:
	Sandbox() 
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox() 
	{
	}
};

Cober::Application* Cober::CreateApplication()
{
	return new Sandbox();
}
