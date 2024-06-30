#include "acpch.h"

#include "Application.h"

#include "Events/ApplicationEvent.h"

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogApplication);

	Application::Application()
	{
		Window = std::unique_ptr<IWindow>(IWindow::Create());
	}

	Application::~Application()
	{
	}


	void Application::Run()
	{
		while (bRunning)
		{
			Window->OnUpdate();
		}
	}
}