#include <Engine.h>
#include <Cober/EntryPoint.h>

#include "Sandbox2D.h"

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
