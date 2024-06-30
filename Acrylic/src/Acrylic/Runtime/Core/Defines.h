#pragma once

#ifdef AC_PLATFORM_WINDOWS
	#ifdef AC_BUILD_DLL
		#define ACRYLIC_API __declspec(dllexport)
	#else
		#define ACRYLIC_API __declspec(dllimport)
	#endif
#else
	#error Unsupported platform!
#endif

#define BREAK() (__debugbreak())

#ifdef AC_ENABLE_ASSERTS
	#define AC_ASSERT(x, ...)                                   \
		{                                                       \
			if (!(x))                                           \
			{                                                   \
				BREAK();                                        \
			}                                                   \
		}
#else
	#define AC_ASSERT(x, ...)
#endif
