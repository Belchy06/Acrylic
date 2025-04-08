#pragma once

#if AC_PLATFORM_WINDOWS
	#include "Windows/WindowsPlatformString.h"
#else
	#include "Generic/GenericPlatformString.h"
#endif