#pragma once

#include "ApplicationCore/Window.h"
#include "Core/Logging/Logging.h"
#include "Renderer/OpenGL/OpenGLContext.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogWindowsWindow, Log);

	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const WindowProperties& Properties);
		virtual ~WindowsWindow() override;

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return Data.Width; }
		virtual uint32_t GetHeight() const override { return Data.Height; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& Callback) override { Data.EventCallback = Callback; }
		virtual void SetVSync(bool bEnabled) override;
		virtual bool IsVSync() override;

		virtual void* GetNativeWindow() const override { return Window; }

	private:
		virtual void Init(const WindowProperties& Properties);
		virtual void Shutdown();

	private:
		GLFWwindow* Window;

		ICommandContext* GraphicsContext;

		struct WindowData
		{
			std::string Title = "";
			uint32_t	Width = 1280;
			uint32_t	Height = 720;
			bool		bVsync = false;

			EventCallbackFn EventCallback;
		};

		WindowData Data;
	};
} // namespace Acrylic