#pragma once
#include "Cober/Core/Core.h"

#ifdef CB_PLATFORM_WINDOWS

extern Cober::Application* Cober::CreateApplication();

int main(int argc, char** argv) 
{
	CB_PROFILE_BEGIN_SESSION("Startup", "CoberProfile-Startup.json");
	auto app = Cober::CreateApplication();
	CB_PROFILE_END_SESSION();

	CB_PROFILE_BEGIN_SESSION("Runtime", "CoberProfile-Runtime.json");
	app->Run();
	CB_PROFILE_END_SESSION();

	CB_PROFILE_BEGIN_SESSION("Shutdown", "CoberProfile-Shutdown.json");
	_CrtDumpMemoryLeaks();
	delete app;
	CB_PROFILE_END_SESSION();
	
	return 0;
}

#endif