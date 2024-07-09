#pragma once

#include "Core/Memory.h"
#include "Renderer/Texture.h"

namespace Acrylic
{
	class IViewport
	{
	public:
		virtual ~IViewport() = default;

		virtual TSharedPtr<ITexture> GetBackbuffer() = 0;
		virtual void				 Resize(uint32_t Width, uint32_t Height) = 0;

		// TEMP
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static TSharedPtr<IViewport> Create(uint32_t Width, uint32_t Height);
	};
} // namespace Acrylic
