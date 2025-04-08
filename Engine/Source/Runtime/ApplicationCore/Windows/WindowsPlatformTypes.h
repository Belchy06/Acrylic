#pragma once

#include "Generic/GenericPlatformTypes.h"

#if AC_PLATFORM_WINDOWS
/**
 * Windows specific types
 **/
struct ACRYLIC_API WindowsPlatformTypes : public GenericPlatformTypes
{
};

typedef WindowsPlatformTypes PlatformTypes;
#endif