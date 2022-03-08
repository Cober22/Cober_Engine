#include <Engine.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Cober::Layer
{
public:
	ExampleLayer() 
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//SDL_LogInfo(0, "ExampleLayer::Update");
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(SDL_Event& event) override
	{
		SDL_LogInfo(0, (char*)event.text.text);
	}
};

class Sandbox : public Cober::Application {

public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Cober::Application* Cober::CreateApplication()
{
	return new Sandbox();
}
