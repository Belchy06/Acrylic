#pragma once

#include "Core/Containers/String.h"
#include "Core/Core.h"
#include "Core/Memory.h"
#include "Renderer/RHI.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class ACRYLIC_API IBulkData
	{
	public:
		virtual const void* GetData() const = 0;
		virtual uint32_t	GetSize() const = 0;
		virtual void		Discard() = 0;
	};

	// Simple IBulkData implementation for uploading pixel data managed by the caller
	class ACRYLIC_API UploadArrayBulkData : public IBulkData
	{
	public:
		UploadArrayBulkData(const void* InData, uint32_t InSizeInBytes)
			: Data(InData)
			, SizeInBytes(InSizeInBytes)
		{
		}

		virtual const void* GetData() const final
		{
			return Data;
		}

		virtual uint32_t GetSize() const final
		{
			return SizeInBytes;
		}

		virtual void Discard() override
		{
		}

	private:
		const void* const Data;
		const uint32_t	  SizeInBytes;
	};

	struct ACRYLIC_API TextureDesc
	{
		uint32_t	 Width;
		uint32_t	 Height;
		EPixelFormat PixelFormat;
	};

	struct ACRYLIC_API CreateTextureDesc : public TextureDesc
	{
		/* Optional initial data to fill the resource with. */
		IBulkData* BulkData = nullptr;
	};

	class ACRYLIC_API ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual uint32_t GetID() const = 0;

		virtual const TextureDesc& GetDesc() { return Desc; };

		virtual void Bind(uint32_t Slot = 0) const = 0;

	protected:
		TextureDesc Desc;
	};

	class ACRYLIC_API Texture2D : public ITexture
	{
	public:
		virtual const glm::vec2* const GetTexCoords() { return TextureCoords; }

		static TSharedPtr<Texture2D> Create(CreateTextureDesc& Desc);
		static TSharedPtr<Texture2D> Create(const String& Path);

	protected:
		glm::vec2 TextureCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	};

	class ACRYLIC_API SubTexture2D : public Texture2D
	{
	public:
		// Load a full texture and extract only the section you need
		static TSharedPtr<SubTexture2D> Create(const String& Path, const glm::vec2& Coords, const glm::vec2& Size);
		// Create a sub texture from an already loaded texture
		static TSharedPtr<SubTexture2D> Create(TSharedPtr<Texture2D> Texture, const glm::vec2& Coords, const glm::vec2& Size);
	};
} // namespace Acrylic
