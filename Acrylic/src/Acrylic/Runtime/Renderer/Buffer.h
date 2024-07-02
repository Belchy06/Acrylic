#pragma once

namespace Acrylic
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IVertexBuffer* Create(float* Vertices, uint32_t Count);
	};

	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;

		static IIndexBuffer* Create(uint32_t* Indices, uint32_t Count);
	};
}