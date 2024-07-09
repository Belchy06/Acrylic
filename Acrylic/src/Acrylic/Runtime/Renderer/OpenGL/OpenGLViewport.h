#pragma once

#include "Renderer/Texture.h"
#include "Renderer/Viewport.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class OpenGLViewport : public IViewport
	{
	public:
		OpenGLViewport(uint32_t Width, uint32_t Height);
		virtual ~OpenGLViewport();

		virtual TSharedPtr<ITexture> GetBackbuffer() override { return Backbuffer; }
		virtual void				 Resize(uint32_t Width, uint32_t Height) override;

		virtual void				 Bind() override;
		virtual void				 Unbind() override;
	private:
		uint32_t			 RendererId = 0;
		TSharedPtr<ITexture> Backbuffer = nullptr;
	};
} // namespace Acrylic
