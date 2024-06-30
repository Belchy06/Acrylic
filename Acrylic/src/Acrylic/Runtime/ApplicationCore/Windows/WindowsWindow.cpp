#include "acpch.h"
#include "WindowsWindow.h"

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogWindowsWindow);

	static bool bGLFWInitialized = false;

	IWindow* IWindow::Create(const WindowProperties& Properties)
	{
		return new WindowsWindow(Properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& Properties)
	{
		Init(Properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& Properties)
	{
		Data.Title = Properties.Title;
		Data.Width = Properties.Width;
		Data.Height = Properties.Height;

		AC_LOG(LogWindowsWindow, Log, "Creating window \"{0}\", ({1}, {2})", Properties.Title, Properties.Width, Properties.Height);

		if (!bGLFWInitialized)
		{
			int Success = glfwInit();
			AC_ASSERT(Success);

			bGLFWInitialized = true;
		}

		Window = glfwCreateWindow(static_cast<int>(Data.Width), static_cast<int>(Data.Height), Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(Window);
		glfwSetWindowUserPointer(Window, &Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(Window);
	}

	void WindowsWindow::SetVSync(bool bEnabled)
	{
		glfwSwapInterval(bEnabled ? 1 : 0);
		Data.bVsync = bEnabled;
	}

	bool WindowsWindow::IsVSync()
	{
		return Data.bVsync;
	}
}
