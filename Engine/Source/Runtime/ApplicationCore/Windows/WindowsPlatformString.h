#pragma once

#include "Generic/GenericPlatformString.h"
#include "PlatformTypes.h"

#include <tchar.h>

#if AC_PLATFORM_WINDOWS
struct ACRYLIC_API WindowsPlatformString : public GenericPlatformString
{
    static size_t Strlen(const WIDECHAR* Str)
    {
        return _tcslen(Str);
    }

    static size_t Strlen(const ANSICHAR* Str)
    {
        return strlen(Str);
    }

    static size_t Strlen(const UTF8CHAR* Str)
    {
        return Strlen((const ANSICHAR*)Str);
    }
};

typedef WindowsPlatformString PlatformString;
#endif