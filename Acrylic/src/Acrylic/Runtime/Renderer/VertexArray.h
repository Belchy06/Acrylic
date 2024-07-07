#pragma once

#include "Core/Memory.h"
#include "Renderer/Buffer.h"

namespace Acrylic
{
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const TSharedPtr<IVertexBuffer>& VertexBuffer) = 0;
		virtual void SetIndexBuffer(const TSharedPtr<IIndexBuffer>& IndexBuffer) = 0;

		virtual const TVector<TSharedPtr<IVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const TSharedPtr<IIndexBuffer>&				  GetIndexBuffer() const = 0;

		static TSharedPtr<IVertexArray> Create();
	};
} // namespace Acrylic