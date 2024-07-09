#include "acpch.h"
#include "Texture.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

namespace Acrylic
{
	TSharedPtr<Texture2D> Texture2D::Create(CreateTextureDesc& Desc)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLTexture2D>(Desc);
			default:
				return nullptr;
		}
	}

	TSharedPtr<Texture2D> Texture2D::Create(const String& Path)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLTexture2D>(Path);
			default:
				return nullptr;
		}
	}

	TSharedPtr<SubTexture2D> SubTexture2D::Create(const String& Path, const glm::vec2& Coords, const glm::vec2& Size)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLSubTexture2D>(Texture2D::Create(Path), Coords, Size);
			default:
				return nullptr;
		}
	}

	TSharedPtr<SubTexture2D> SubTexture2D::Create(TSharedPtr<Texture2D> Texture, const glm::vec2& Coords, const glm::vec2& Size)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLSubTexture2D>(Texture, Coords, Size);
			default:
				return nullptr;
		}
	}
} // namespace Acrylic
