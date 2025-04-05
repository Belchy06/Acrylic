#pragma once

#if AC_WINDOWS
	#include "Windows/WindowsPlatform.h"
#else
	#include "Generic/GenericPlatform.h"
typedef GenericPlatformTypes PlatformTypes;
#endif

typedef PlatformTypes::uint8	 uint8;
typedef PlatformTypes::uint16	 uint16;
typedef PlatformTypes::uint32	 uint32;
typedef PlatformTypes::uint64	 uint64;
typedef PlatformTypes::int8		 int8;
typedef PlatformTypes::int16	 int16;
typedef PlatformTypes::int32	 int32;
typedef PlatformTypes::int64	 int64;
typedef PlatformTypes::ANSICHAR	 ANSICHAR;
typedef PlatformTypes::WIDECHAR	 WIDECHAR;
typedef PlatformTypes::UTF8CHAR	 UTF8CHAR;
typedef PlatformTypes::CHAR16	 CHAR16;
typedef PlatformTypes::UTF32CHAR UTF32CHAR;