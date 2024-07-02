#include "acpch.h"
#include "ImGuiLayer.h"

#include "ApplicationCore/Application.h"

#define IMGUI_IMPL_API
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
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
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		// IO.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		// IO.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGui::StyleColorsDark();
		ImGuiStyle& Style = ImGui::GetStyle();
		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			Style.WindowRounding = 0.f;
			Style.Colors[ImGuiCol_WindowBg].w = 1.f;
		}

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(Timestep ts)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.DeltaTime = ts.GetSeconds();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() 
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.DisplaySize = ImVec2(static_cast<float>(Application::GetWindow().GetWidth()), static_cast<float>(Application::GetWindow().GetHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* BackupContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(BackupContext);
		}
	}
} // namespace Acrylic
