#pragma once

#include "Core/Core.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Acrylic
{
	class ACRYLIC_API IInput
	{
	public:
		static bool IsKeyPressed(int KeyCode) { return Singleton->IsKeyPressedImpl(KeyCode); }
		static bool IsMouseButtonPressed(int Button) { return Singleton->IsMouseButtonPressedImpl(Button); }
		static std::pair<double, double> GetMousePostion() { return Singleton->GetMousePostionImpl(); }
		static double GetMouseX() { return Singleton->GetMouseXImpl(); }
		static double GetMouseY() { return Singleton->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int KeyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int Button) = 0;
		virtual std::pair<double, double> GetMousePostionImpl() = 0;
		virtual double GetMouseXImpl() = 0;
		virtual double GetMouseYImpl() = 0;

	private:
		static IInput* Singleton;
	};
}