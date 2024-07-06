#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IShader* Create(const String& VertexSrc, const String& FragmentSrc);
	};
}