#pragma once

#include "Core/Containers/String.h"
#include "Core/Core.h"
#include "Core/Memory.h"

namespace Acrylic
{
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t Slot = 0) const = 0;
	};

	class Texture2D : public ITexture
	{
	public:
		static TSharedPtr<Texture2D> Create(const String& Path);
	};
} // namespace Acrylic
