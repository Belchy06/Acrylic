#pragma once

#include "ApplicationCore/Application.h"

#ifdef AC_PLATFORM_WINDOWS
	extern Acrylic::Application* Acrylic::CreateApplication();

	int main(int argc, char** argv)
	{
		Acrylic::Application* application = Acrylic::CreateApplication();
		application->Run();
		delete application;

		return 0;
	}
#else
	#error Unsupported platform!
#endif