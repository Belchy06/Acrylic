#include "Sandbox2D.h"

#include <glm/gtc/type_ptr.hpp>

DEFINE_LOG_CATEGORY(LogSandbox2D);

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, CameraController(Acrylic::MakeUnique<Acrylic::OrthographicCameraController>(1280.f / 720.f, true))
{
}

void Sandbox2D::OnAttach()
{
	Texture = Acrylic::Texture2D::Create("assets/textures/Test.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Acrylic::Timestep ts)
{
	AC_PROFILE_FUNCTION()

	// Update
	{
		AC_PROFILE_SCOPE("Update")
		CameraController->OnUpdate(ts);
	}

	{
		AC_PROFILE_SCOPE("Render")
		// Render
		Acrylic::GCommandListExecutor->SetClearColour({ 1.f, 0.1f, 0.1f, 1.f });
		Acrylic::GCommandListExecutor->Clear();

		Acrylic::Renderer2D::BeginScene(CameraController->GetCamera());
		Acrylic::Renderer2D::DrawQuad({ //
			.Position = { 0.f, 0.f, -.1f },
			.Size = { 10.f, 10.f },
			.Rotation = 180.f,
			.Colour = glm::vec4(1.f), // glm::vec4(Color, 1.f),
			.Texture = Texture,
			.TilingFactor = 10.f });

		glm::vec2 Size = { 10.f / NumQuads[0], 10.f / NumQuads[1] };

		for (int y = 0; y < NumQuads[1]; y++)
		{
			for (int x = 0; x < NumQuads[0]; x++)
			{
				float PosX = ((((float)x - 1) * ((5.f - Size.x) - (-5.f + Size.x))) / (NumQuads[0] - 1.f)) + (-5.f + Size.x);
				float PosY = ((((float)y - 1) * ((5.f - Size.y) - (-5.f + Size.y))) / (NumQuads[1] - 1.f)) + (-5.f + Size.y);

				glm::vec4 Colour = { PosX, 0.4f, PosY, 1.f };

				Acrylic::Renderer2D::DrawQuad({ //
					.Position = { PosX, PosY, 0.f },
					.Size = Size,
					.Colour = Colour });
			}
		}

		Acrylic::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	AC_PROFILE_FUNCTION()

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Color", glm::value_ptr(Color));
	ImGui::SliderInt2("Num Quads", NumQuads, 1, 100);
	ImGui::End();

	const Acrylic::Renderer2D::Stats* Statistics = Acrylic::Renderer2D::GetStats();
	ImGui::Begin("Stats");
	ImGui::Text("Renderer2D Stats");
	ImGui::Text("Draw Calls: %d", Statistics->DrawCalls);
	ImGui::Text("Quads:      %d", Statistics->QuadCount);
	ImGui::Text("Vertices    %d", Statistics->QuadCount * 4);
	ImGui::Text("Indices:    %d", Statistics->QuadCount * 6);
	ImGui::End();
}

void Sandbox2D::OnEvent(Acrylic::Event& e)
{
	CameraController->OnEvent(e);
}
