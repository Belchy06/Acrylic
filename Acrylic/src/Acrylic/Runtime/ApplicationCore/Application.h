#pragma once

#include "ApplicationCore/Window.h"
#include "Core/Logging/Logging.h"
#include "Layers/LayerStack.h"
#include "Layers/ImGui/ImGuiLayer.h"

// TEMP
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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

		std::unique_ptr<IWindow>	Window;
		std::unique_ptr<ImGuiLayer> GUILayer;
		bool						bRunning = true;
		LayerStack					Stack;

		static Application* Singleton;

		std::vector<std::pair<std::shared_ptr<IShader>, std::shared_ptr<IVertexArray>>> Shaders;
	};

	Application* CreateApplication();
} // namespace Acrylic