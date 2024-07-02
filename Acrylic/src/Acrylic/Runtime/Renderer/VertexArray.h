#pragma once

#include "Renderer/Buffer.h"

namespace Acrylic
{
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& VertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& IndexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IIndexBuffer>&			   GetIndexBuffers() const = 0;

		static IVertexArray* Create();
	};
} // namespace Acrylic