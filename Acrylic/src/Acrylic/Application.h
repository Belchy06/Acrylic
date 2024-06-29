#pragma once

#include "Core.h"

namespace Acrylic
{
	class ACRYLIC_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		void Run();
	};

	Application* CreateApplication();
} // namespace Acrylic