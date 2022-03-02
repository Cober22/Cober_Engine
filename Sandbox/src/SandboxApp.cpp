
#include <Engine.h>

class Sandbox : public Cober::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}

};

Cober::Application* Cober::CreateApplication()
{
	return new Sandbox();
}
