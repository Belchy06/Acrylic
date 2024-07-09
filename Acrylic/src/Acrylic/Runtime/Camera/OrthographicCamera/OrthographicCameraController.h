#pragma once

#include "Camera/CameraController.h"
#include "Camera/OrthographicCamera/OrthographicCamera.h"

namespace Acrylic
{
	class ACRYLIC_API OrthographicCameraController : public ICameraController
	{
	public:
		OrthographicCameraController(float AspectRatio, bool bRotate = false);

		virtual void					OnUpdate(Timestep ts) override;
		virtual void					OnEvent(Event& e) override;
		virtual CameraControllerBounds& GetBounds() override { return Bounds; }
		virtual TSharedPtr<ICamera>		GetCamera() override { return Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		bool  bRotate;
		float AspectRatio;
		float ZoomLevel = 1.f;

		CameraControllerBounds Bounds;

		TSharedPtr<OrthographicCamera> Camera;

		glm::vec3 CameraPosition = { 0.f, 0.f, 0.f };
		float	  CameraRotation = 0.f;

		float MoveSpeed = 5.f;
		float RotateSpeed = 180.f;
		float ZoomSpeed = .5f;
	};
} // namespace Acrylic
