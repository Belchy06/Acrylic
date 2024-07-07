#include "acpch.h"
#include "Buffer.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLBuffer.h"

namespace Acrylic
{
	TSharedPtr<IVertexBuffer> IVertexBuffer::Create(uint32_t Size)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLVertexBuffer>(Size);
			default:
				return nullptr;
		}
	}

	TSharedPtr<IVertexBuffer> IVertexBuffer::Create(float* Vertices, uint32_t Count)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLVertexBuffer>(Vertices, Count);
			default:
				return nullptr;
		}
	}

	TSharedPtr<IIndexBuffer> IIndexBuffer::Create(uint32_t* Indices, uint32_t Count)
	{
		switch (GRenderInterface)
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLIndexBuffer>(Indices, Count);
			default:
				return nullptr;
		}
	}
} // namespace Acrylic