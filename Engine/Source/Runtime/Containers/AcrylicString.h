#pragma once

#include "Platform.h"

#include <type_traits>

template <typename CharType>
class TString
{
public:
	template <typename T>
	TString(T& Arr)
		requires(std::is_array_v<T>)
	{
	}

	// Constructor for pointers. ie const wchar_t*
	template <typename T>
	TString(T&& Ptr)
		requires(!std::is_array_v<T>)
	{
	}
};

using WideString = TString<WIDECHAR>;
using AnsiString = TString<ANSICHAR>;
using Utf8String = TString<UTF8CHAR>;