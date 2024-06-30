#pragma once

#include "Core/Logging/Logging.h"
#include "ApplicationCore/Window.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogApplication, Log);

	class ACRYLIC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<IWindow> Window;
		bool					 bRunning = true;
	};

	Application* CreateApplication();
} // namespace Acrylic