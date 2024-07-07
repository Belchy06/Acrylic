#pragma once

#include "Core/Core.h"
#include "Core/Misc/EnumClassFlags.h"

namespace Acrylic
{
	enum class ERenderInterface : uint8_t
	{
		None = 0,
		OpenGL
	};

	ENUM_CLASS_FLAGS(ERenderInterface)

	extern inline ERenderInterface GRenderInterface = ERenderInterface::None;

	enum class EPixelFormat : uint8_t
	{
		Unknown = 0,
		RGBA8,
		RGB8,
	};
}
