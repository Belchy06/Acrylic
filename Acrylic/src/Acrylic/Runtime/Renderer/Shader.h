#pragma once

#include "acpch.h"

namespace Acrylic
{
	class Shader
	{
	public:
		Shader(const std::string& VertexSrc, const std::string& FragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t RendererId;
	};
}