#pragma once

#include "ApplicationCore/Application.h"

#ifdef AC_PLATFORM_WINDOWS
extern Application* CreateApplication();

int main(int argc, char** argv)
{
	Application* App = CreateApplication();

	App->Run();
	
	delete App;

	return 0;
}
#else
	#error Unsupported platform!
#endif