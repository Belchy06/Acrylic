#pragma once

#include "Acrylic.h"

#include <imgui.h>

DECLARE_LOG_CATEGORY_EXTERN(LogSandbox2D, Log);

class Sandbox2D : public Acrylic::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Acrylic::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Acrylic::Event& e) override;

private:
	Acrylic::TUniquePtr<Acrylic::OrthographicCameraController> CameraController;
	Acrylic::TSharedPtr<Acrylic::Texture2D>					   Texture;

	glm::vec3 Color = { 1.f, 1.f, 1.f };
	int		  NumQuads[2] = { 10, 10 }; // x, y
};
