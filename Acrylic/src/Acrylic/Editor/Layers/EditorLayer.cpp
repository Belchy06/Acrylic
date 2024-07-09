#include "EditorLayer.h"

#include "Core/Instrumentation.h"
#include "ApplicationCore/Application.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/CommandList.h"

#include <imgui.h>

namespace Acrylic
{
	EditorLayer::EditorLayer()
		: Layer("Editor")
		, Viewport(IViewport::Create(1280, 720))
		, CameraController(MakeUnique<OrthographicCameraController>(1280.f / 720.f, true))
	{
		ViewportSize = { Viewport->GetBackbuffer()->GetDesc().Width, Viewport->GetBackbuffer()->GetDesc().Height };

		ViewportBounds[0] = glm::vec2(0.f);
		ViewportBounds[1] = glm::vec2(0.f);
	}

	void EditorLayer::OnAttach()
	{
		AC_PROFILE_FUNCTION()
	}

	void EditorLayer::OnDetach()
	{
		AC_PROFILE_FUNCTION()
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		AC_PROFILE_FUNCTION()

		{
			AC_PROFILE_SCOPE("Update")
			// Update
			if (bViewportHovered)
			{
				CameraController->OnUpdate(ts);
			}
		}

		{
			AC_PROFILE_SCOPE("Render")
			// Render
			Viewport->Bind();

			GCommandListExecutor->SetClearColour({ 0.1f, 0.1f, 0.1f, 1.f });
			GCommandListExecutor->Clear();

			Renderer2D::BeginScene(CameraController->GetCamera());
			Renderer2D::DrawQuad({
				.Position = { 0.f, 0.f, .5f },	 //
				.Size = { 1.f, 1.f },			 //
				.Colour = { 1.f, 1.f, 1.f, 1.f } //
			});
			Renderer2D::EndScene();

			Viewport->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		AC_PROFILE_FUNCTION()
#if 1
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
		auto ViewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto ViewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto ViewportOffset = ImGui::GetWindowPos();
		ViewportBounds[0] = { ViewportMinRegion.x + ViewportOffset.x, ViewportMinRegion.y + ViewportOffset.y };
		ViewportBounds[1] = { ViewportMaxRegion.x + ViewportOffset.x, ViewportMaxRegion.y + ViewportOffset.y };

		bViewportFocused = ImGui::IsWindowFocused();
		bViewportHovered = ImGui::IsWindowHovered();

		Application::GetApplication().BlockImGuiEvents(!bViewportHovered);

		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();
		if (ViewportSize != *((glm::vec2*)&ViewportPanelSize))
		{
			Viewport->Resize(ViewportPanelSize.x, ViewportPanelSize.y);
			ViewportSize = { ViewportPanelSize.x, ViewportPanelSize.y };

			// Trigger a fake window resize event to cause the camera controller to up
			WindowResizeEvent FakeEvent(ViewportSize.x, ViewportSize.y);
			CameraController->OnEvent(FakeEvent);
		}

		uint64_t TextureID = Viewport->GetBackbuffer()->GetID();
		ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ ViewportSize.x, ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
#else
		const Renderer2D::Stats* Statistics = Renderer2D::GetStats();
		ImGui::Begin("Stats");
		ImGui::Text("Renderer2D Stats");
		ImGui::Text("Draw Calls: %d", Statistics->DrawCalls);
		ImGui::Text("Quads:      %d", Statistics->QuadCount);
		ImGui::Text("Vertices    %d", Statistics->QuadCount * 4);
		ImGui::Text("Indices:    %d", Statistics->QuadCount * 6);
		ImGui::End();
#endif
	}

	void EditorLayer::OnEvent(Event& e)
	{
		AC_PROFILE_FUNCTION()

		CameraController->OnEvent(e);
	}

} // namespace Acrylic
