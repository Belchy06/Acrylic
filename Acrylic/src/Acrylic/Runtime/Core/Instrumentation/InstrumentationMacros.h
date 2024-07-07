#pragma once

#include "Core/Defines.h"
#include "Core/Instrumentation/Instrumentation.h"

#if AC_ENABLE_PROFILING
	#define AC_PROFILE_SCOPE(Name) ::Acrylic::InstrumentationTimer Timer##__LINE__(Name);
	#define AC_PROFILE_FUNCTION() AC_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define AC_PROFILE_SCOPE(Name)
	#define AC_PROFILE_FUNCTION()
#endif