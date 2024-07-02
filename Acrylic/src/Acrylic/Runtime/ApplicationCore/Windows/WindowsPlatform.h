#pragma once

#include "ApplicationCore/Platform.h"

namespace Acrylic
{
	class WindowsPlatform : public IPlatform
	{
	protected:
		virtual double GetTimeImpl() override;
	};
} // namespace Acrylic
