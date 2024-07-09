#pragma once

#include "Camera/Camera.h"
#include "Core/Memory.h"
#include "Core/Timestep.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Acrylic
{
	struct ACRYLIC_API CameraControllerBounds
	{
		float Left, Right;
		float Top, Bottom;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class ACRYLIC_API ICameraController
	{
	public:
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		virtual CameraControllerBounds& GetBounds() = 0;
		virtual TSharedPtr<ICamera> GetCamera() = 0;
	};
} // namespace Acrylic