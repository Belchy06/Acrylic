#pragma once

#include "PlatformTypes.h"

#include <type_traits>

template <typename CharType>
class TStringView : private TString<CharType>
{
public:
	template <typename T>
	TStringView(T& Arr)
		requires(std::is_array_v<T>)
	{
	}

	// Constructor for pointers. ie const wchar_t*
	template <typename T>
	TStringView(T&& Ptr)
		requires(!std::is_array_v<T>)
	{
	}
};

using WideStringView = TStringView<WIDECHAR>;
using AnsiStringView = TStringView<ANSICHAR>;
using Utf8StringView = TStringView<UTF8CHAR>;