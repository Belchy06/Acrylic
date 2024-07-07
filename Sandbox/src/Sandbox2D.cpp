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
	// Update
	CameraController->OnUpdate(ts);

	// Render
	Acrylic::GCommandListExecutor->SetClearColour({ 1.f, 0.1f, 0.1f, 1.f });
	Acrylic::GCommandListExecutor->Clear();

	Acrylic::Renderer2D::BeginScene(CameraController->GetCamera());
	Acrylic::Renderer2D::DrawQuad({ -1.f, 0.f }, { .8f, .8f }, glm::vec4(Color, 1.f));
	Acrylic::Renderer2D::DrawQuad({  .5f,-.5f }, { .5f, .75f }, glm::vec4(Color, 1.f));
	Acrylic::Renderer2D::DrawQuad({ .0f, .0f, -0.1f }, { 10.f, 10.f }, Texture);
	Acrylic::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Color", glm::value_ptr(Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Acrylic::Event& e)
{
	CameraController->OnEvent(e);
}
