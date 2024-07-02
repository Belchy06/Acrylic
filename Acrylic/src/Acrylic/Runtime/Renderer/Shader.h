#pragma once

#include "acpch.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniformMat4(const std::string& Name, const glm::mat4& Matrix) = 0;

		static IShader* Create(const std::string& VertexSrc, const std::string& FragmentSrc);
	};
}