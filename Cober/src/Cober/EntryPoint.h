#pragma once

#ifdef CB_PLATFORM_WINDOWS

extern Cober::Application* Cober::CreateApplication();

int main(int argc, char** argv) 
{
	auto app = Cober::CreateApplication();
	app->Run();
	delete app;

	
	return 0;
}

#endif