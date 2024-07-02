#pragma once

#include "Renderer/Buffer.h"

namespace Acrylic
{
	class OpenGLVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* Vertices, uint32_t Count);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void				SetLayout(const BufferLayout& InLayout) { Layout = InLayout; }
		virtual const BufferLayout& GetLayout() const { return Layout; }

	private:
		uint32_t	 RendererId;
		BufferLayout Layout;
	};

	class OpenGLIndexBuffer : public IIndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* Indices, uint32_t Count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void	 Bind() const override;
		virtual void	 Unbind() const override;
		virtual uint32_t GetCount() const override { return Count; }

	private:
		uint32_t RendererId;
		uint32_t Count;
	};
} // namespace Acrylic