#pragma once

#include "Core/Containers/String.h"
#include "Core/Core.h"
#include "Core/Memory.h"
#include "Renderer/RHI.h"

namespace Acrylic
{
	class ACRYLIC_API ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t Slot = 0) const = 0;
	};

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

	struct ACRYLIC_API CreateTextureDesc
	{
		uint32_t	 Width;
		uint32_t	 Height;
		EPixelFormat PixelFormat;

		/* Optional initial data to fill the resource with. */
		IBulkData* BulkData = nullptr;
	};

	class ACRYLIC_API Texture2D : public ITexture
	{
	public:
		static TSharedPtr<Texture2D> Create(CreateTextureDesc& Desc);
		static TSharedPtr<Texture2D> Create(const String& Path);
	};
} // namespace Acrylic
