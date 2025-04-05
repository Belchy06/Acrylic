#pragma once

#include "Generic/GenericPlatform.h"

#if AC_PLATFORM_WINDOWS
/**
 * Windows specific types
 **/
struct ACRYLIC_API WindowsPlatformTypes : public GenericPlatformTypes
{
};

typedef WindowsPlatformTypes PlatformTypes;
#endif