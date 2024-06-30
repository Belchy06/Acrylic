#pragma once

#include "Core.h"
#include "Logging/Logging.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogApplication, Log);

	class ACRYLIC_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		void Run();
	};

	Application* CreateApplication();
} // namespace Acrylic