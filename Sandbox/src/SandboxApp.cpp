
#include <Engine.h>

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

	void OnEvent(SDL_Event& event) override
	{
<<<<<<< Updated upstream
		if (&event != NULL)
			SDL_LogInfo(0, (char*)event.text.text);
=======
		//SDL_LogInfo(0, (char*)event.text.text);
>>>>>>> Stashed changes
	}
};

class Sandbox : public Cober::Application {

public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Cober::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Cober::Application* Cober::CreateApplication()
{
	return new Sandbox();
}
