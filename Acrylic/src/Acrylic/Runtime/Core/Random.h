#pragma once

#include "Core/Core.h"

#include <random>

namespace Acrylic
{
	class ACRYLIC_API Random
	{
	public:
		static void Init()
		{
			RandomEngine.seed(std::random_device()());
		}

		static float Float()
		{
			return (float)Distribution(RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

	private:
		static inline std::mt19937 RandomEngine;
		static inline std::uniform_int_distribution<std::mt19937::result_type> Distribution;
	};
} // namespace Acrylic