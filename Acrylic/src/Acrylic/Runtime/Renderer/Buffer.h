#pragma once

#include "Core/Containers/Array.h"
#include "Core/Containers/String.h"
#include "Core/Core.h"

namespace Acrylic
{
	enum class EDataType : uint8_t
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint8_t SizeOf(EDataType Type)
	{
		switch (Type)
		{
			case EDataType::Float:
				return 4;
			case EDataType::Float2:
				return 8;
			case EDataType::Float3:
				return 12;
			case EDataType::Float4:
				return 16;
			case EDataType::Mat3:
				return 36;
			case EDataType::Mat4:
				return 64;
			case EDataType::Int:
				return 4;
			case EDataType::Int2:
				return 8;
			case EDataType::Int3:
				return 12;
			case EDataType::Int4:
				return 16;
			case EDataType::Bool:
				return 1;
			case EDataType::None:
				return 0;
			default:
				AC_ASSERT(false);
				return 0;
		}
	}

	struct BufferElement
	{
		String Name;
		EDataType	Type;
		uint32_t	Offset;
		uint32_t	Size;
		bool		bNormalized;

		BufferElement()
			: Name("")
			, Type(EDataType::None)
			, Size(SizeOf(Type))
			, Offset(0)
			, bNormalized(false)
		{
		}

		BufferElement(EDataType Type, const String& Name, bool bNormalized = false)
			: Name(Name)
			, Type(Type)
			, Size(SizeOf(Type))
			, Offset(0)
			, bNormalized(bNormalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case EDataType::Float:
				case EDataType::Int:
					return 1;
				case EDataType::Float2:
				case EDataType::Int2:
					return 2;
				case EDataType::Float3:
				case EDataType::Int3:
					return 3;
				case EDataType::Float4:
				case EDataType::Int4:
					return 4;
				case EDataType::Mat3:
					return 9;
				case EDataType::Mat4:
					return 16;
				case EDataType::Bool:
					return 1;
				case EDataType::None:
					return 0;
				default:
					AC_ASSERT(false);
					return 0;
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout()
			: Elements({})
			, Stride(0)
		{
		}

		BufferLayout(const std::initializer_list<BufferElement>& Elements)
			: Elements(Elements)
			, Stride(0)
		{
			CalculateOffsetsAndStride();
		}

		const TArray<BufferElement>& GetElements() const { return Elements; }
		uint32_t						  GetStride() const { return Stride; }

		TArray<BufferElement>::iterator		 begin() { return Elements.begin(); }
		TArray<BufferElement>::iterator end() { return Elements.end(); }

		TArray<BufferElement>::const_iterator	   begin() const { return Elements.begin(); }
		TArray<BufferElement>::const_iterator end() const { return Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			Stride = 0;
			uint32_t Offset = 0;
			for (BufferElement& Element : Elements)
			{
				Element.Offset = Offset;
				Offset += Element.Size;
				Stride += Element.Size;
			}
		}

	private:
		TArray<BufferElement> Elements;
		uint32_t				   Stride;
	};

	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void				SetLayout(const BufferLayout& Layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static IVertexBuffer* Create(float* Vertices, uint32_t Count);
	};

	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual void	 Bind() const = 0;
		virtual void	 Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;

		static IIndexBuffer* Create(uint32_t* Indices, uint32_t Count);
	};
} // namespace Acrylic