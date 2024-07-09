#include "acpch.h"
#include "Viewport.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLViewport.h"

namespace Acrylic
{
	TSharedPtr<IViewport> IViewport::Create(uint32_t Width, uint32_t Height)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLViewport>(Width, Height);
			default:
				return nullptr;
		}
	}
} // namespace Acrylic
