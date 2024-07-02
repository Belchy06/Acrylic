#pragma once

#include "Input/Input.h"

namespace Acrylic
{
	class WindowsInput : public IInput
	{
	protected:
		virtual bool   IsKeyPressedImpl(int KeyCode) override;
		virtual bool   IsMouseButtonPressedImpl(int Button) override;
		virtual std::pair<double, double> GetMousePostionImpl() override;
		virtual double GetMouseXImpl() override;
		virtual double GetMouseYImpl() override;
	};
} // namespace Acrylic
