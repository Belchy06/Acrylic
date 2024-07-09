#include "acpch.h"

#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Platform.h"

#include <glfw/glfw3.h>

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogApplication);

	Application* Application::Singleton = nullptr;

	Application::Application(const String& Name)
	{
		Singleton = this;
		Window = TUniquePtr<IWindow>(IWindow::Create(WindowProperties(Name)));
		Window->SetEventCallback(AC_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		Renderer2D::Init();

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
		Dispatcher.Dispatch<WindowResizeEvent>(AC_BIND_EVENT_FN(Application::OnWindowResized));

		AC_LOG(LogApplication, VeryVerbose, "{0}", e.ToString());

		for (TVector<Layer*>::iterator It = Stack.end(); It != Stack.begin();)
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

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			bMinimised = true;
			return false;
		}

		bMinimised = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
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
			Renderer2D::ResetStats();

			float	 Time = static_cast<float>(IPlatform::GetTime() / 1000.f);
			Timestep Step = Time - LastFrameTime;
			LastFrameTime = Time;

			if (!bMinimised)
			{
				for (Layer* l : Stack)
				{
					l->OnUpdate(Step);
				}
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

	void Application::Close()
	{
		bRunning = false;
	}

	void Application::BlockEvents()
	{
	}
} // namespace Acrylic