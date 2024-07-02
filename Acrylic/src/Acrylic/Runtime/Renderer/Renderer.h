#pragma once

#include "Core/Core.h"
#include "Core/Misc/EnumClassFlags.h"

namespace Acrylic
{
	enum class ERenderAPI : uint8_t
	{
		None = 0,
		OpenGL
	};

	ENUM_CLASS_FLAGS(ERenderAPI)

	class Renderer
	{
	public:
		static FORCEINLINE ERenderAPI GetAPI() { return RenderAPI; }

	private:
		static ERenderAPI RenderAPI;
	};
} // namespace Acrylic
