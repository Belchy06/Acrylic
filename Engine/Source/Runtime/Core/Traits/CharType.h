#pragma once

#include "ApplicationCore/PlatformTypes.h"

// NOTE (william.belcher): This class deviates in style from the rest of Acrylic but does so to better match the std API

// clang-format off
namespace Acrylic
{
    template <typename T> struct is_char_type { enum { value = false }; };
    template <> struct is_char_type<WIDECHAR> { enum { value = true }; };
    template <> struct is_char_type<ANSICHAR> { enum { value = true }; };
    template <> struct is_char_type<UTF8CHAR> { enum { value = true }; };
    
    template <typename T> struct is_char_type<const          T> : is_char_type<T> { };
    template <typename T> struct is_char_type<      volatile T> : is_char_type<T> { };
    template <typename T> struct is_char_type<const volatile T> : is_char_type<T> { };
	// clang-format on

	template <typename T>
	constexpr inline bool is_char_type_v = is_char_type<T>::value;
} // namespace Acrylic