#pragma once

#if AC_PLATFORM_WINDOWS
	#include "Windows/WindowsPlatformString.h"
#else
	#include "Generic/GenericPlatformString.h"

// clang-format off
	struct DummyPlatformString : public GenericPlatformString
	{
		static size_t Strlen(const WIDECHAR* Str)
		{
			static_assert(false, "DummyPlatformString Strlen(const WIDECHAR* Str) is not implemented!");
		}

		static size_t Strlen(const ANSICHAR* Str)
		{
			static_assert(false, "DummyPlatformString Strlen(const ANSICHAR* Str) is not implemented!");
		}

		static size_t Strlen(const UTF8CHAR* Str)
		{
			static_assert(false, "DummyPlatformString Strlen(const UTF8CHAR* Str) is not implemented!");
		}
	}

	typedef DummyPlatformString PlatformString;
// clang-format on
#endif