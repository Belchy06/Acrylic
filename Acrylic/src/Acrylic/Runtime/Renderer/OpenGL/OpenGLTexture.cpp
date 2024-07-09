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

	EPixelFormat GetPixelFormat(GLenum OpenGLPixelFormat)
	{
		switch (OpenGLPixelFormat)
		{
			case GL_RGBA8:
				return EPixelFormat::RGBA8;
			case GL_RGB8:
				return EPixelFormat::RGB8;
		}

		AC_ASSERT(false);
		return EPixelFormat::Unknown;
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

	OpenGLTexture2D::OpenGLTexture2D(CreateTextureDesc& InDesc)
		: RendererId(0)
	{
		AC_PROFILE_FUNCTION()

		GLenum InternalFormat = GetOpenGLPixelFormat(InDesc.PixelFormat);
		GLenum DataFormat = GetOpenGLDataFormat(InDesc.PixelFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererId);
		glTextureStorage2D(RendererId, 1, InternalFormat, InDesc.Width, InDesc.Height);

		glTextureParameteri(RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (InDesc.BulkData != nullptr)
		{
			glTextureSubImage2D(RendererId, 0, 0, 0, InDesc.Width, InDesc.Height, DataFormat, GL_UNSIGNED_BYTE, InDesc.BulkData->GetData());

			InDesc.BulkData->Discard();
			InDesc.BulkData = nullptr;
		}

		Desc.Width = InDesc.Width;
		Desc.Height = InDesc.Height;
		Desc.PixelFormat = Desc.PixelFormat;
	}

	OpenGLTexture2D::OpenGLTexture2D(const String& Path)
		: Path(Path)
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
		glTextureStorage2D(RendererId, 1, InternalFormat, LoadedWidth, LoadedHeight);

		glTextureParameteri(RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(RendererId, 0, 0, 0, LoadedWidth, LoadedHeight, DataFormat, GL_UNSIGNED_BYTE, Data);

		stbi_image_free(Data);

		Desc.Width = LoadedWidth;
		Desc.Height = LoadedHeight;
		Desc.PixelFormat = GetPixelFormat(InternalFormat);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &RendererId);
	}

	uint32_t OpenGLTexture2D::GetID() const
	{
		return RendererId;
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
		glm::vec2 Min = { ((Coords.x) * Size.x) / Texture->GetDesc().Width, ((Coords.y + 1) * Size.y) / Texture->GetDesc().Height };
		glm::vec2 Max = { ((Coords.x + 1) * Size.x) / Texture->GetDesc().Width, (Coords.y * Size.y) / Texture->GetDesc().Height };

		TextureCoords[0] = { Min.x, Min.y };
		TextureCoords[1] = { Max.x, Min.y };
		TextureCoords[2] = { Max.x, Max.y };
		TextureCoords[3] = { Min.x, Max.y };
	}
} // namespace Acrylic
