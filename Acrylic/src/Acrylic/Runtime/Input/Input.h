#pragma once

#include "Core/Core.h"
#include "Core/Containers/Pair.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Acrylic
{
	class ACRYLIC_API Input
	{
	public:
		static bool					 IsKeyPressed(int KeyCode);
		static bool					 IsMouseButtonPressed(int Button);
		static TPair<double, double> GetMousePostion();
		static double				 GetMouseX();
		static double				 GetMouseY();
	};
} // namespace Acrylic