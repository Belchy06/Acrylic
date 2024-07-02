#pragma once

#include "Core/Core.h"

namespace Acrylic
{
	class ACRYLIC_API Timestep
	{
	public:
		Timestep(float Time = 0.f)
			: Time(Time)
		{
		}

		operator float() const { return Time; }

		float GetSeconds() const { return Time;  }
		float GetMilliseconds() const { return Time * 1000.f; }
	private:
		float Time;
	};
}