#pragma once

#include "ApplicationCore/Window.h"
#include "Core/Logging.h"
#include "Core/Memory.h"
#include "Core/Timestep.h"
#include "Layers/LayerStack.h"
#include "Layers/ImGui/ImGuiLayer.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogApplication, Log)

	class ACRYLIC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Close();
		void BlockEvents();

		void OnEvent(Event& e);

		void PushLayer(Layer* InLayer);
		void PushOverlay(Layer* InOverlay);

		static FORCEINLINE Application& GetApplication() { return *Singleton; }
		static FORCEINLINE IWindow&		GetWindow() { return *Singleton->Window; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		TUniquePtr<IWindow>	   Window;
		TUniquePtr<ImGuiLayer> GUILayer;
		bool				   bRunning = true;
		bool				   bMinimised = false;
		LayerStack			   Stack;

		float LastFrameTime = 0.f;

		static Application* Singleton;
	};

	Application* CreateApplication();
} // namespace Acrylic