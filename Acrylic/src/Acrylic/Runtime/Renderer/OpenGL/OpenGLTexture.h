#pragma once

#include "Core/Logging.h"
#include "Renderer/Texture.h"

namespace Acrylic
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(CreateTextureDesc& Desc);
		OpenGLTexture2D(const String& Path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetID() const override;
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void Bind(uint32_t Slot = 0) const override;

	private:
		String Path;
		uint32_t Width;
		uint32_t Height;

		uint32_t RendererId;
	};
} // namespace Acrylic
