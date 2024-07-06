#pragma once

#include "Camera/Camera.h"
#include "Core/Memory.h"
#include "Core/Timestep.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Acrylic
{
	class ACRYLIC_API ICameraController
	{
	public:
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		virtual TSharedPtr<ICamera> GetCamera() = 0;
	};
} // namespace Acrylic