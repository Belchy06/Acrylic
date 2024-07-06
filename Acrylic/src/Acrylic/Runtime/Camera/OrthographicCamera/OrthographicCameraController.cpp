#include "acpch.h"
#include "OrthographicCameraController.h"
#include "Input/Input.h"

namespace Acrylic
{
	OrthographicCameraController::OrthographicCameraController(float AspectRatio, bool bRotate)
		: bRotate(bRotate)
		, AspectRatio(AspectRatio)
	{
		Camera = MakeShared<OrthographicCamera>(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel);
		CameraPosition = Camera->GetPosition();
		CameraRotation = Camera->GetRotation();
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (IInput::IsKeyPressed(AC_KEY_W))
		{
			CameraPosition.y += MoveSpeed * ts;
		}
		else if (IInput::IsKeyPressed(AC_KEY_S))
		{
			CameraPosition.y -= MoveSpeed * ts;
		}

		if (IInput::IsKeyPressed(AC_KEY_A))
		{
			CameraPosition.x -= MoveSpeed * ts;
		}
		else if (IInput::IsKeyPressed(AC_KEY_D))
		{
			CameraPosition.x += MoveSpeed * ts;
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
		Camera->SetProjection(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		Camera->SetProjection(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel);

		return false;
	}
} // namespace Acrylic