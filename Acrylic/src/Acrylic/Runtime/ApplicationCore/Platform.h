#pragma once

#include "Core/Core.h"

namespace Acrylic
{
	class ACRYLIC_API IPlatform
	{
	public:
		static double GetTime() { return Singleton->GetTimeImpl(); }

	protected:
		virtual double GetTimeImpl() = 0;

	private:
		static IPlatform* Singleton;
	};
}