#pragma once

#include "Camera/OrthographicCamera/OrthographicCameraController.h"
#include "Layers/Layer.h"
#include "Renderer/Viewport.h"

namespace Acrylic
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool bViewportFocused = false;
		bool bViewportHovered = false;
		glm::vec2 ViewportSize = { 0.0f, 0.0f };
		glm::vec2 ViewportBounds[2];

		TSharedPtr<IViewport> Viewport;

		TUniquePtr<OrthographicCameraController> CameraController;
	};
} // namespace Acrylic
