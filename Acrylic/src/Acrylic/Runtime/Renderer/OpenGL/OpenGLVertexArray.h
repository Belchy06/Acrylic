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

		virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& VertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& IndexBuffer) override;

		virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const override { return VertexBuffers; }
		virtual const std::shared_ptr<IIndexBuffer>&			   GetIndexBuffers() const override { return IndexBuffer;  }

	private:
		uint32_t RendererId;

		std::vector<std::shared_ptr<IVertexBuffer>> VertexBuffers;
		std::shared_ptr<IIndexBuffer>				IndexBuffer;
	};
} // namespace Acrylic