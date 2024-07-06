#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class ACRYLIC_API ICamera
	{
	public:
		virtual const glm::mat4& GetProjectionMatrix() = 0;
		virtual const glm::mat4& GetViewMatrix() = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() = 0;
		virtual const glm::vec3& GetPosition() = 0;
		virtual float			 GetRotation() = 0;

		virtual void SetProjection(float Left, float Right, float Bottom, float Top) = 0;
		virtual void SetPosition(const glm::vec3& Position) = 0;
		virtual void SetRotation(float Rotation) = 0;
	};
} // namespace Acrylic