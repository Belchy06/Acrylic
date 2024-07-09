#include "EditorLayer.h"

#include "Core/Instrumentation.h"
#include "ApplicationCore/Application.h"
#include "Renderer/Renderer2D.h"

#include <imgui.h>

namespace Acrylic
{
	EditorLayer::EditorLayer()
		: Layer("Editor")
	{
	}

	void EditorLayer::OnAttach()
	{
		AC_PROFILE_FUNCTION()
	}

	void EditorLayer::OnDetach()
	{
		AC_PROFILE_FUNCTION()
	}

	void EditorLayer::OnUpdate(Acrylic::Timestep ts)
	{
		AC_PROFILE_FUNCTION()
	}

	void EditorLayer::OnImGuiRender()
	{
		AC_PROFILE_FUNCTION()

		// Note: Switch this to true to enable dockspace
		static bool				  bDockspaceOpen = true;
		static bool				  bFullscreen = true;
		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (bFullscreen)
		{
			ImGuiViewport* ViewportPtr = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ViewportPtr->Pos);
			ImGui::SetNextWindowSize(ViewportPtr->Size);
			ImGui::SetNextWindowViewport(ViewportPtr->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (DockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		{
			WindowFlags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &bDockspaceOpen, WindowFlags);
		ImGui::PopStyleVar();

		if (bFullscreen)
		{
			ImGui::PopStyleVar(2);
		}

		// DockSpace
		ImGuiIO&	IO = ImGui::GetIO();
		ImGuiStyle& Style = ImGui::GetStyle();
		float		MinWinSizeX = Style.WindowMinSize.x;
		Style.WindowMinSize.x = 370.0f;
		if (IO.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID DockspaceID = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
		}

		Style.WindowMinSize.x = MinWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					Application::GetApplication().Close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		const Renderer2D::Stats* Statistics = Renderer2D::GetStats();
		ImGui::Begin("Stats");
		ImGui::Text("Renderer2D Stats");
		ImGui::Text("Draw Calls: %d", Statistics->DrawCalls);
		ImGui::Text("Quads:      %d", Statistics->QuadCount);
		ImGui::Text("Vertices    %d", Statistics->QuadCount * 4);
		ImGui::Text("Indices:    %d", Statistics->QuadCount * 6);
		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		/*auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		bViewportFocused = ImGui::IsWindowFocused();
		bViewportHovered = ImGui::IsWindowHovered();

		Application::GetApplication().GetImGuiLayer()->BlockEvents(!bViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });*/

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Acrylic::Event& e)
	{
		AC_PROFILE_FUNCTION()
	}

} // namespace Acrylic
