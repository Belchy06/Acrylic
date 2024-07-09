#include "acpch.h"
#include "ApplicationCore/Platform.h"

namespace Acrylic
{
	double Platform::GetTime()
	{
		LARGE_INTEGER Frequency;
		if (!QueryPerformanceFrequency(&Frequency))
		{
			AC_ASSERT(false);
		}

		LARGE_INTEGER Ticks;
		if (!QueryPerformanceCounter(&Ticks))
		{
			AC_ASSERT(false);
		}
		return double(Ticks.QuadPart) / double(Frequency.QuadPart / 1000);
	}
}
