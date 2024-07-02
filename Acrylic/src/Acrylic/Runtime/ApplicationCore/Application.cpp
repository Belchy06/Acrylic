#include "acpch.h"

#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Renderer.h"
#include "Platform.h"

#include <glfw/glfw3.h>

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogApplication);

	Application* Application::Singleton = nullptr;

	Application::Application()
	{
		Singleton = this;
		Window = std::unique_ptr<IWindow>(IWindow::Create());
		Window->SetEventCallback(AC_BIND_EVENT_FN(Application::OnEvent));

		Stack.PushOverlay(new ImGuiLayer());

		LastFrameTime = static_cast<float>(IPlatform::GetTime());
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowCloseEvent>(AC_BIND_EVENT_FN(Application::OnWindowClosed));

		AC_LOG(LogApplication, VeryVerbose, "{0}", e.ToString());

		for (std::vector<Layer*>::iterator It = Stack.end(); It != Stack.begin();)
		{
			(*--It)->OnEvent(e);
			if (e.IsHandled())
			{
				break;
			}
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		bRunning = false;
		return true;
	}

	void Application::PushLayer(Layer* InLayer)
	{
		Stack.PushLayer(InLayer);
	}

	void Application::PushOverlay(Layer* InOverlay)
	{
		Stack.PushOverlay(InOverlay);
	}

	void Application::Run()
	{
		while (bRunning)
		{
			float Time = static_cast<float>(IPlatform::GetTime() / 1000.f);
			Timestep Step = Time - LastFrameTime;
			LastFrameTime = Time;

			for (Layer* l : Stack)
			{
				l->OnUpdate(Step);
			}

			GUILayer->Begin();
			for (Layer* l : Stack)
			{
				l->OnImGuiRender();
			}
			GUILayer->End();

			Window->OnUpdate();
		}
	}
} // namespace Acrylic