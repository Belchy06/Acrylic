#pragma once

#include "PlatformTypes.h"
#include "PlatformString.h"
#include "Core/Traits/CharType.h"

#include <type_traits>

template <typename CharType>
class TString
{
public:
	TString() = default;
	TString(TString&&) = default;
	TString(const TString&) = default;
	TString& operator=(TString&&) = default;
	TString& operator=(const TString&) = default;

	/** Construct from null-terminated C string or nullptr  */
	template <typename T, size_t N>
	[[nodiscard]] inline TString(T (&Arr)[N])
	{
		ConstructFromPtrAndSize(Arr, N);
	}

	/** Construct from string literal  */
	template <size_t N>
	[[nodiscard]] inline TString(const ANSICHAR (&Arr)[N])
	{
		ConstructFromLiteral(Arr, N);
	}

	template <size_t N>
	[[nodiscard]] inline TString(const WIDECHAR (&Arr)[N])
	{
		ConstructFromLiteral(Arr, N);
	}

	template <size_t N>
	[[nodiscard]] inline TString(const UTF8CHAR (&Arr)[N])
	{
		ConstructFromLiteral(Arr, N);
	}

	/** Construct from null-terminated C string or nullptr  */
	template <typename T>
	[[nodiscard]] inline TString(T&& Ptr)
		requires(std::is_pointer_v<std::decay_t<T>> and Acrylic::is_char_type_v<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>)
	{
		if (!Ptr || !*Ptr)
		{
			return;
		}
		ConstructFromPtrAndSize(Ptr, PlatformString::Strlen(Ptr));
	}

	inline const CharType* operator*() const
	{
		return Data;
	}

	inline const size_t GetNum() const
	{
		return NumChars;
	}

private:
	template <typename T>
	void ConstructFromLiteral(const T* Ptr, size_t Num)
	{
		if (!Ptr || Num == 0 || !*Ptr)
		{
			return;
		}

		NumChars = Num;
		Data = Ptr;
	}

	template <typename T>
	void ConstructFromPtrAndSize(const T* Ptr, size_t Num)
	{
		if (!Ptr || Num == 0 || !*Ptr)
		{
			return;
		}

		NumChars = Num;
		Data = new CharType[NumChars];
		memcpy(const_cast<CharType*>(Data), Ptr, NumChars * sizeof(*Ptr));
	}

private:
	const CharType* Data = nullptr;
	size_t			NumChars = 0;
};