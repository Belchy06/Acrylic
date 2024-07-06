#pragma once

#include <memory>

namespace Acrylic
{
	template <typename T>
	using TUniquePtr = std::unique_ptr<T>;

	template <class T, class... Args>
	TUniquePtr<T> MakeUnique(Args&&... InArgs)
	{
		return std::make_unique<T>(std::forward<Args>(InArgs)...);
	}

	template <typename T>
	using TSharedPtr = std::shared_ptr<T>;

	template <class T, class... Args>
	TSharedPtr<T> MakeShared(Args&&... InArgs)
	{
		return std::make_shared<T>(std::forward<Args>(InArgs)...);
	}

	template <typename T>
	using TWeakPtr = std::weak_ptr<T>;
}