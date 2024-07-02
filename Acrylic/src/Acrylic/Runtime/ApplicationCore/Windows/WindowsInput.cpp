#include "acpch.h"
#include "WindowsInput.h"

#include "ApplicationCore/Application.h"

#include <glfw/glfw3.h>

namespace Acrylic
{
	IInput* IInput::Singleton = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int KeyCode)
	{
		IWindow& Window = Application::GetWindow();

		int State = glfwGetKey(static_cast<GLFWwindow*>(Window.GetNativeWindow()), KeyCode);
		return State == GLFW_PRESS || State == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int Button)
	{
		IWindow& Window = Application::GetWindow();

		int State = glfwGetMouseButton(static_cast<GLFWwindow*>(Window.GetNativeWindow()), Button);
		return State == GLFW_PRESS;
	}

	std::pair<double, double> WindowsInput::GetMousePostionImpl()
	{
		IWindow& Window = Application::GetWindow();

		double XPos, YPos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Window.GetNativeWindow()), &XPos, &YPos);

		return { XPos, YPos };
	}

	double WindowsInput::GetMouseXImpl()
	{
		auto[XPos, YPos] = GetMousePostionImpl();
		return XPos;
	}

	double WindowsInput::GetMouseYImpl()
	{
		auto [XPos, YPos] = GetMousePostionImpl();
		return YPos;
	}
}
