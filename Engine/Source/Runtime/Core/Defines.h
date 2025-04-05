#pragma once

#include <functional>

#ifdef AC_PLATFORM_WINDOWS
	#if AC_DYNAMIC_LINK
		#ifdef AC_BUILD_DLL
			#define ACRYLIC_API __declspec(dllexport)
		#else
			#define ACRYLIC_API __declspec(dllimport)
		#endif
	#else
		#define ACRYLIC_API
	#endif
#else
	#error Unsupported platform!
#endif

#define BREAK() (__debugbreak())

#ifdef AC_DEBUG
	#define AC_ENABLE_ASSERTS 1
	#define AC_ENABLE_PROFILING 1
#endif

#if AC_ENABLE_ASSERTS
	#define AC_ASSERT(x, ...) \
		{                     \
			if (!(x))         \
			{                 \
				BREAK();      \
			}                 \
		}
#else
	#define AC_ASSERT(x, ...)
#endif

#define AC_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)