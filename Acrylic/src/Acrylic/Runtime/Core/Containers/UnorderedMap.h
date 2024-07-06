#pragma once

#include <unordered_map>

namespace Acrylic
{
	template <typename T1, typename T2>
	using TUnorderedMap = std::unordered_map<T1, T2>;
}