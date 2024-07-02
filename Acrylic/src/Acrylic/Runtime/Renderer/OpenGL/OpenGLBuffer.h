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

	private:
		uint32_t RendererId;
	};

	class OpenGLIndexBuffer : public IIndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* Indices, uint32_t Count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override;

	private:
		uint32_t RendererId;
		uint32_t Count;
	};
} // namespace Acrylic