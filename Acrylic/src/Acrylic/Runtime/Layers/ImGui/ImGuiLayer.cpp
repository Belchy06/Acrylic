#include "acpch.h"
#include "ImGuiLayer.h"

#include "Renderer/OpenGL/ImGuiOpenGLRenderer.h"
#include "ApplicationCore/Application.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Acrylic
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& IO = ImGui::GetIO();
		IO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		IO.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Keyboard mapping. ImGui will use those indices to peek into the IO.KeysDown[] array.
		IO.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		IO.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		IO.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		IO.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		IO.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		IO.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		IO.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		IO.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		IO.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		IO.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		IO.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		IO.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		IO.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		IO.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		IO.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		IO.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		IO.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		IO.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		IO.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		IO.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		IO.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.DisplaySize = ImVec2(static_cast<float>(Application::GetWindow().GetWidth()), static_cast<float>(Application::GetWindow().GetHeight()));

		float Time = (float)glfwGetTime();
		IO.DeltaTime = DeltaTime > 0.f ? (Time - DeltaTime) : 1.f / 60.0f;
		DeltaTime = Time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<MouseButtonPressedEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		Dispatcher.Dispatch<MouseButtonReleasedEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		Dispatcher.Dispatch<MouseMovedEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		Dispatcher.Dispatch<MouseScrolledEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		Dispatcher.Dispatch<KeyPressedEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		Dispatcher.Dispatch<KeyReleasedEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		Dispatcher.Dispatch<KeyCharEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnKeyCharEvent));
		Dispatcher.Dispatch<WindowResizeEvent>(AC_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.KeysDown[e.GetKeyCode()] = true;

		IO.KeyCtrl = IO.KeysDown[GLFW_KEY_LEFT_CONTROL] || IO.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		IO.KeyShift = IO.KeysDown[GLFW_KEY_LEFT_SHIFT] || IO.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		IO.KeyAlt = IO.KeysDown[GLFW_KEY_LEFT_ALT] || IO.KeysDown[GLFW_KEY_RIGHT_ALT];
		IO.KeySuper = IO.KeysDown[GLFW_KEY_LEFT_SUPER] || IO.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyCharEvent(KeyCharEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		int		 KeyCode = e.GetKeyCode();
		if (KeyCode > 0 && KeyCode < 0x100000)
		{
			IO.AddInputCharacter(static_cast<unsigned short>(KeyCode));
		}

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MousePos = ImVec2(e.GetDeltaX(), e.GetDeltaY());

		return false;
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseWheel += e.GetY();
		IO.MouseWheelH += e.GetX();

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		IO.DisplayFramebufferScale = ImVec2(1.f, 1.f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

} // namespace Acrylic
