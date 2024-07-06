#pragma once

#include <memory>

namespace Acrylic
{
	template <typename T>
	using TUniquePtr = std::unique_ptr<T>;

	template <typename T>
	using TSharedPtr = std::shared_ptr<T>;

	template <typename T>
	using TWeakPtr = std::weak_ptr<T>;
}