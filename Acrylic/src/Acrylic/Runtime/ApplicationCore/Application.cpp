#include "acpch.h"

#include "Application.h"

#include "Events/ApplicationEvent.h"

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogApplication);

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		Window = std::unique_ptr<IWindow>(IWindow::Create());
		Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		AC_LOG(LogApplication, VeryVerbose, "{0}", e.ToString());
	}
	
	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		bRunning = false;
		return true;
	}

	void Application::Run()
	{
		while (bRunning)
		{
			Window->OnUpdate();
		}
	}
}