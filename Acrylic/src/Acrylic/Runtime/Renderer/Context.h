#pragma once

#include "Core/Core.h"
#include "Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class ACRYLIC_API ICommandContext
	{
	public:
		virtual ~ICommandContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
} // namespace Acrylic
