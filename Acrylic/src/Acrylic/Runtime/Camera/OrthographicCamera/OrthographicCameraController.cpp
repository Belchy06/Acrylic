#include "acpch.h"
#include "OrthographicCameraController.h"
#include "Input/Input.h"

namespace Acrylic
{
	OrthographicCameraController::OrthographicCameraController(float AspectRatio, bool bRotate)
		: bRotate(bRotate)
		, AspectRatio(AspectRatio)
		, Bounds({ -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel })
		, Camera(MakeShared<OrthographicCamera>(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top))
	{
		CameraPosition = Camera->GetPosition();
		CameraRotation = Camera->GetRotation();
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (IInput::IsKeyPressed(AC_KEY_W))
		{
			CameraPosition.x -= sin(glm::radians(CameraRotation)) * MoveSpeed * ts;
			CameraPosition.y += cos(glm::radians(CameraRotation)) * MoveSpeed * ts;

		}
		else if (IInput::IsKeyPressed(AC_KEY_S))
		{
			CameraPosition.x += sin(glm::radians(CameraRotation)) * MoveSpeed * ts;
			CameraPosition.y -= cos(glm::radians(CameraRotation)) * MoveSpeed * ts;
		}

		if (IInput::IsKeyPressed(AC_KEY_A))
		{
			CameraPosition.x -= cos(glm::radians(CameraRotation)) * MoveSpeed * ts;
			CameraPosition.y -= sin(glm::radians(CameraRotation)) * MoveSpeed * ts;
		}
		else if (IInput::IsKeyPressed(AC_KEY_D))
		{
			CameraPosition.x += cos(glm::radians(CameraRotation)) * MoveSpeed * ts;
			CameraPosition.y += sin(glm::radians(CameraRotation)) * MoveSpeed * ts;
		}

		Camera->SetPosition(CameraPosition);	

		if (bRotate)
		{
			if (IInput::IsKeyPressed(AC_KEY_Q))
			{
				CameraRotation += RotateSpeed * ts;
			}
			else if (IInput::IsKeyPressed(AC_KEY_E))
			{
				CameraRotation -= RotateSpeed * ts;
			}

			Camera->SetRotation(CameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);

		Dispatcher.Dispatch<MouseScrolledEvent>(AC_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		Dispatcher.Dispatch<WindowResizeEvent>(AC_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		ZoomLevel -= e.GetY() * ZoomSpeed;
		ZoomLevel = std::max(ZoomLevel, 0.25f);
		Bounds = { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel };
		Camera->SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		Bounds = { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel };
		Camera->SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);

		return false;
	}
} // namespace Acrylic