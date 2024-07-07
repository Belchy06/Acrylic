#pragma once

#include "Renderer/VertexArray.h"

namespace Acrylic
{
	class OpenGLVertexArray : public IVertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const TSharedPtr<IVertexBuffer>& VertexBuffer) override;
		virtual void SetIndexBuffer(const TSharedPtr<IIndexBuffer>& IndexBuffer) override;

		virtual const TVector<TSharedPtr<IVertexBuffer>>& GetVertexBuffers() const override { return VertexBuffers; }
		virtual const TSharedPtr<IIndexBuffer>&				  GetIndexBuffer() const override { return IndexBuffer; }

	private:
		uint32_t RendererId;

		TVector<TSharedPtr<IVertexBuffer>> VertexBuffers;
		TSharedPtr<IIndexBuffer>			   IndexBuffer;
	};
} // namespace Acrylic