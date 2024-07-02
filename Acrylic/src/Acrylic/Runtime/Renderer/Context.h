#pragma once

#include "Core/Core.h"

namespace Acrylic
{
	class ACRYLIC_API IGraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
} // namespace Acrylic
