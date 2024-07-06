#pragma once

#include "Core/Core.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class ACRYLIC_API ICommandListExecutor
	{
	public:
		virtual ~ICommandListExecutor() = default;

		virtual void Clear() = 0;
		virtual void SetClearColour(const glm::vec4& Colour) = 0;
		virtual void DrawIndexed(const TSharedPtr<IVertexArray>& VertexArray) = 0;
	};

	extern inline ICommandListExecutor* GCommandListExecutor = nullptr;
}