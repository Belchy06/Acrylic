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

		virtual void Bind(uint32_t Slot = 0) const override;

	private:
		String Path;

		uint32_t RendererId;
	};

	class OpenGLSubTexture2D : public SubTexture2D
	{
	public:
		OpenGLSubTexture2D(TSharedPtr<Texture2D> Texture, const glm::vec2& Coords, const glm::vec2& Size);
		virtual ~OpenGLSubTexture2D() = default;

		virtual uint32_t GetID() const override { return Texture->GetID(); }
		virtual const TextureDesc& GetDesc() override { return Texture->GetDesc(); }

		virtual void Bind(uint32_t Slot = 0) const override { Texture->Bind(Slot); }

	private:
		TSharedPtr<Texture2D> Texture;
	};
} // namespace Acrylic
