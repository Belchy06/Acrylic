#include "acpch.h"
#include "WindowsWindow.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogWindowsWindow);

	static bool bGLFWInitialized = false;

	static void GLFWErrorCallback(int Error, const char* Description)
	{
		AC_LOG(LogWindowsWindow, Log, "GLFW Error: ({0}), {1}", Error, std::string(Description));
	}

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
			glfwSetErrorCallback(GLFWErrorCallback);

			bGLFWInitialized = true;
		}

		Window = glfwCreateWindow(static_cast<int>(Data.Width), static_cast<int>(Data.Height), Data.Title.c_str(), nullptr, nullptr);

		GraphicsContext = new OpenGLContext(Window);
		GraphicsContext->Init();


		glfwSetWindowUserPointer(Window, &Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(Window, [](GLFWwindow* Window, int Width, int Height) {
			WindowData* Data = static_cast<WindowData*>(glfwGetWindowUserPointer(Window));

			Data->Width = Width;
			Data->Height = Height;

			WindowResizeEvent Event(Width, Height);
			Data->EventCallback(Event);
		});

		glfwSetWindowCloseCallback(Window, [](GLFWwindow* Window) {
			WindowData* Data = static_cast<WindowData*>(glfwGetWindowUserPointer(Window));

			WindowCloseEvent Event;
			Data->EventCallback(Event);
		});

		glfwSetKeyCallback(Window, [](GLFWwindow* Window, int Key, int ScanCode, int Action, int Mods) {
			WindowData* Data = static_cast<WindowData*>(glfwGetWindowUserPointer(Window));

			switch (Action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent Event(Key, 0);
					Data->EventCallback(Event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent Event(Key);
					Data->EventCallback(Event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent Event(Key, 1);
					Data->EventCallback(Event);
					break;
				}
			}
		});

		glfwSetCharCallback(Window, [](GLFWwindow* Window, unsigned int Key) {
			WindowData* Data = static_cast<WindowData*>(glfwGetWindowUserPointer(Window));

			KeyCharEvent Event(Key);
			Data->EventCallback(Event);
		});

		glfwSetMouseButtonCallback(Window, [](GLFWwindow* Window, int Button, int Action, int Mods) {
			WindowData* Data = static_cast<WindowData*>(glfwGetWindowUserPointer(Window));

			switch (Action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent Event(Button);
					Data->EventCallback(Event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent Event(Button);
					Data->EventCallback(Event);
					break;
				}
			}
		});

		glfwSetScrollCallback(Window, [](GLFWwindow* Window, double X, double Y) {
			WindowData* Data = static_cast<WindowData*>(glfwGetWindowUserPointer(Window));

			MouseScrolledEvent Event(X, Y);
			Data->EventCallback(Event);
		});

		glfwSetCursorPosCallback(Window, [](GLFWwindow* Window, double X, double Y) {
			WindowData* Data = static_cast<WindowData*>(glfwGetWindowUserPointer(Window));

			MouseMovedEvent Event(X, Y);
			Data->EventCallback(Event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		GraphicsContext->SwapBuffers();
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
} // namespace Acrylic
