#include "acpch.h"
#include "WindowsPlatform.h"

namespace Acrylic
{
	IPlatform* IPlatform::Singleton = new WindowsPlatform();

	double WindowsPlatform::GetTimeImpl()
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
