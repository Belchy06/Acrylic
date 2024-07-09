#include "acpch.h"
#include "Input/Input.h"

#include "ApplicationCore/Application.h"

#include <glfw/glfw3.h>

namespace Acrylic
{
	bool Input::IsKeyPressed(int KeyCode)
	{
		IWindow& Window = Application::GetWindow();

		int State = glfwGetKey(static_cast<GLFWwindow*>(Window.GetNativeWindow()), KeyCode);
		return State == GLFW_PRESS || State == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int Button)
	{
		IWindow& Window = Application::GetWindow();

		int State = glfwGetMouseButton(static_cast<GLFWwindow*>(Window.GetNativeWindow()), Button);
		return State == GLFW_PRESS;
	}

	std::pair<double, double> Input::GetMousePostion()
	{
		IWindow& Window = Application::GetWindow();

		double XPos, YPos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Window.GetNativeWindow()), &XPos, &YPos);

		return { XPos, YPos };
	}

	double Input::GetMouseX()
	{
		auto[XPos, YPos] = GetMousePostion();
		return XPos;
	}

	double Input::GetMouseY()
	{
		auto [XPos, YPos] = GetMousePostion();
		return YPos;
	}
}
