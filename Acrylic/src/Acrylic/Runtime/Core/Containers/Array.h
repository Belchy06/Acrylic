#pragma once

#include <array>
#include <vector>

namespace Acrylic
{
	template<typename T, size_t N>
	using TArray = std::array<T, N>;

	template<typename T>
	using TVector = std::vector<T>;
}