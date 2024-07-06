#include "acpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Acrylic
{
	OpenGLTexture2D::OpenGLTexture2D(const String& Path)
		: Path(Path)
		, Width(0)
		, Height(0)
		, RendererId(0)
	{
		stbi_set_flip_vertically_on_load(1);

		int		 LoadedWidth, LoadedHeight, LoadedChannels;
		stbi_uc* Data = stbi_load(Path.c_str(), &LoadedWidth, &LoadedHeight, &LoadedChannels, 0);
		if (!Data)
		{
			return;
		}

		Width = LoadedWidth;
		Height = LoadedHeight;

		GLenum InternalFormat = 0, DataFormat = 0;
		if (LoadedChannels == 4)
		{
			InternalFormat = GL_RGBA8;
			DataFormat = GL_RGBA;
		}
		else if (LoadedChannels == 3)
		{
			InternalFormat = GL_RGB8;
			DataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererId);
		glTextureStorage2D(RendererId, 1, InternalFormat, Width, Height);

		glTextureParameteri(RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(RendererId, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, Data);

		stbi_image_free(Data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &RendererId);
	}

	uint32_t OpenGLTexture2D::GetWidth() const
	{
		return Width;
	}

	uint32_t OpenGLTexture2D::GetHeight() const
	{
		return Height;
	}

	void OpenGLTexture2D::Bind(uint32_t Slot) const
	{
		glBindTextureUnit(Slot, RendererId);
	}
} // namespace Acrylic
