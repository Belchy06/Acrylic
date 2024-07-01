#pragma once

#include "ApplicationCore/Window.h"
#include "Core/Logging/Logging.h"
#include "Layers/LayerStack.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogApplication, Log)

	class ACRYLIC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* InLayer);
		void PushOverlay(Layer* InOverlay);

		static FORCEINLINE Application& GetApplication() { return *Singleton; }
		static FORCEINLINE IWindow&		GetWindow() { return *Singleton->Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<IWindow> Window;
		bool					 bRunning = true;
		LayerStack				 Stack;

		static Application* Singleton;
	};

	Application* CreateApplication();
} // namespace Acrylic