#include "Sandbox2D.h"

#include <glm/gtc/type_ptr.hpp>

DEFINE_LOG_CATEGORY(LogSandbox2D);

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, CameraController(Acrylic::MakeUnique<Acrylic::OrthographicCameraController>(1280.f / 720.f, true))
	, ParticleSystem(Acrylic::MakeUnique<Acrylic::ParticleSystem>())
{
}

void Sandbox2D::OnAttach()
{
	Texture = Acrylic::Texture2D::Create("assets/textures/Test.png");

	// Default Values
	ParticleProps.Colour.Begin = { 254 / 255.f, 212 / 255.f, 123 / 255.f, 1.f };
	ParticleProps.Colour.End = { 254 / 255.f, 109 / 255.f, 41 / 255.f, 1.f };
	ParticleProps.Size.Begin = .25f;
	ParticleProps.Size.Variation = .1f;
	ParticleProps.Size.End = .0f;
	ParticleProps.Lifetime = 1.f;
	ParticleProps.Velocity.Linear.Initial = { 0.f, -3.f, 0.f };
	ParticleProps.Velocity.Linear.Variation = { 3.f, 1.f, 0.f };
	ParticleProps.Velocity.Angular.Initial = { 0.f, 0.f, 5.f };
	ParticleProps.Velocity.Angular.Variation = { 0.f, 0.f, 0.f };
	ParticleProps.Position.Translation = { 0.f, 0.f, 0.f };
	ParticleProps.Position.Rotation = { 0.f, 0.f, 0.f };
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Acrylic::Timestep ts)
{
	AC_PROFILE_FUNCTION()

	if (Acrylic::IInput::IsMouseButtonPressed(AC_MOUSE_BUTTON_LEFT))
	{
		Acrylic::TPair<double, double> MousePos = Acrylic::IInput::GetMousePostion();
		uint32_t					   Width = Acrylic::Application::GetWindow().GetWidth();
		uint32_t					   Height = Acrylic::Application::GetWindow().GetHeight();

		Acrylic::CameraControllerBounds Bounds = CameraController->GetBounds();
		glm::vec3						CameraPos = CameraController->GetCamera()->GetPosition();

		float X = (MousePos.first / Width) * Bounds.GetWidth() - Bounds.GetWidth() * 0.5;
		float Y = Bounds.GetHeight() * 0.5 - (MousePos.second / Height) * Bounds.GetHeight();
		ParticleProps.Position.Translation = { X + CameraPos.x, Y + CameraPos.y, 0.f };
		for (int i = 0; i < ParticleCount; i++)
		{
			ParticleSystem->Emit(ParticleProps);
		}
	}

	{
		AC_PROFILE_SCOPE("Update")
		// Update
		CameraController->OnUpdate(ts);
		ParticleSystem->OnUpdate(ts);
	}

	{
		AC_PROFILE_SCOPE("Render")
		// Render
		Acrylic::GCommandListExecutor->SetClearColour({ 0.1f, 0.1f, 0.1f, 1.f });
		Acrylic::GCommandListExecutor->Clear();

		ParticleSystem->OnRender(CameraController->GetCamera());
	}
}

void Sandbox2D::OnImGuiRender()
{
	AC_PROFILE_FUNCTION()

	ImGui::Begin("Particle Settings");
	ImGui::ColorEdit3("Particle.Color.Begin", glm::value_ptr(ParticleProps.Colour.Begin));
	ImGui::ColorEdit3("Particle.Color.End", glm::value_ptr(ParticleProps.Colour.End));
	ImGui::SliderFloat("Particle.Size.Begin", &ParticleProps.Size.Begin, 1, 100);
	ImGui::SliderFloat("Particle.Size.Variation", &ParticleProps.Size.Variation, 1, 100);
	ImGui::SliderFloat("Particle.Size.End", &ParticleProps.Size.End, 1, 100);
	ImGui::SliderFloat("Particle.Lifetime", &ParticleProps.Lifetime, 1, 100);
	ImGui::SliderFloat3("ParticleProps.Velocity.Linear.Initial", glm::value_ptr(ParticleProps.Velocity.Linear.Initial), 0.f, 5.f);
	ImGui::SliderFloat3("ParticleProps.Velocity.Linear.Variation", glm::value_ptr(ParticleProps.Velocity.Linear.Variation), 0.f, 5.f);
	ImGui::SliderFloat3("ParticleProps.Velocity.Angular.Initial", glm::value_ptr(ParticleProps.Velocity.Angular.Initial), 0.f, 5.f);
	ImGui::SliderFloat3("ParticleProps.Velocity.Angular.Variation", glm::value_ptr(ParticleProps.Velocity.Angular.Variation), 0.f, 5.f);
	ImGui::SliderFloat3("ParticleProps.Position.Rotation", glm::value_ptr(ParticleProps.Position.Rotation), 0.f, 5.f);
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
