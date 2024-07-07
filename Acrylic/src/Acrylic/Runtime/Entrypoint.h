#pragma once

#include "ApplicationCore/Application.h"
#include "Core/Instrumentation.h"

#ifdef AC_PLATFORM_WINDOWS
	extern Acrylic::Application* Acrylic::CreateApplication();

	int main(int argc, char** argv)
	{
		Acrylic::Instrumentor::Get().BeginSession("Startup", "saved/Startup.json");
		Acrylic::Application* application = Acrylic::CreateApplication();
		Acrylic::Instrumentor::Get().EndSession();

		Acrylic::Instrumentor::Get().BeginSession("Runtime", "saved/Runtime.json");
		application->Run();
		Acrylic::Instrumentor::Get().EndSession();

		Acrylic::Instrumentor::Get().BeginSession("Shutdown", "saved/Shutdown.json");
		delete application;
		Acrylic::Instrumentor::Get().EndSession();

		return 0;
	}
#else
	#error Unsupported platform!
#endif