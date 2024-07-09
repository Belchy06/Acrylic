#include "acpch.h"
#include "OpenGLTexture.h"

#include "Core/Instrumentation.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Acrylic
{
	GLenum GetOpenGLPixelFormat(EPixelFormat PixelFormat)
	{
		switch (PixelFormat)
		{
			case EPixelFormat::RGBA8:
				return GL_RGBA8;
			case EPixelFormat::RGB8:
				return GL_RGB8;
		}

		AC_ASSERT(false);
		return 0;
	}

	GLenum GetOpenGLDataFormat(EPixelFormat PixelFormat)
	{
		switch (PixelFormat)
		{
			case EPixelFormat::RGBA8:
				return GL_RGBA;
			case EPixelFormat::RGB8:
				return GL_RGB;
		}

		AC_ASSERT(false);
		return 0;
	}

	OpenGLTexture2D::OpenGLTexture2D(CreateTextureDesc& Desc)
		: Width(Desc.Width)
		, Height(Desc.Height)
		, RendererId(0)
	{
		AC_PROFILE_FUNCTION()

		GLenum InternalFormat = GetOpenGLPixelFormat(Desc.PixelFormat);
		GLenum DataFormat = GetOpenGLDataFormat(Desc.PixelFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererId);
		glTextureStorage2D(RendererId, 1, InternalFormat, Width, Height);

		glTextureParameteri(RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (Desc.BulkData != nullptr)
		{
			glTextureSubImage2D(RendererId, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, Desc.BulkData->GetData());

			Desc.BulkData->Discard();
			Desc.BulkData = nullptr;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const String& Path)
		: Path(Path)
		, Width(0)
		, Height(0)
		, RendererId(0)
	{
		AC_PROFILE_FUNCTION()

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

	uint32_t OpenGLTexture2D::GetID() const
	{
		return RendererId;
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
		AC_PROFILE_FUNCTION()

		glBindTextureUnit(Slot, RendererId);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	OpenGLSubTexture2D::OpenGLSubTexture2D(TSharedPtr<Texture2D> Texture, const glm::vec2& Coords, const glm::vec2& Size)
		: Texture(Texture)
	{
		glm::vec2 Min = { ((Coords.x) * Size.x) / Texture->GetWidth(), ((Coords.y + 1) * Size.y) / Texture->GetHeight() };
		glm::vec2 Max = { ((Coords.x + 1) * Size.x) / Texture->GetWidth(), (Coords.y * Size.y) / Texture->GetHeight() };

		TextureCoords[0] = { Min.x, Min.y };
		TextureCoords[1] = { Max.x, Min.y };
		TextureCoords[2] = { Max.x, Max.y };
		TextureCoords[3] = { Min.x, Max.y };
	}
} // namespace Acrylic
