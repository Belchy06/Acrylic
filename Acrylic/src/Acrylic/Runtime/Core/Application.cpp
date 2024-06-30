#include "acpch.h"

#include "Application.h"

#include "Events/ApplicationEvent.h"

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogApplication);

	void Application::Run()
	{
		AC_LOG(LogApplication, Log, "Starting Application!");

		WindowResizeEvent e(1280, 720);
		AC_LOG(LogApplication, Log, "{}", e.ToString());
		while (true)
		{

		}
	}
}