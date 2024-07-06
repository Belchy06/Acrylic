#include "acpch.h"
#include "Texture.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

namespace Acrylic
{
	TSharedPtr<Texture2D> Texture2D::Create(const String& Path)
	{
		switch (Renderer::GetRenderInterface())
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLTexture2D>(Path);
			default:
				return nullptr;
		}
	}
} // namespace Acrylic
