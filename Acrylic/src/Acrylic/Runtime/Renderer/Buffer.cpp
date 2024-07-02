#include "acpch.h"
#include "Buffer.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLBuffer.h"

namespace Acrylic
{
	IVertexBuffer* IVertexBuffer::Create(float* Vertices, uint32_t Count)
	{
		switch (Renderer::GetAPI())
		{
			case ERenderAPI::None:
				return nullptr;
			case ERenderAPI::OpenGL:
				return new OpenGLVertexBuffer(Vertices, Count);
			default:
				return nullptr;
		}
	}

	IIndexBuffer* IIndexBuffer::Create(uint32_t* Indices, uint32_t Count)
	{
		switch (Renderer::GetAPI())
		{
			case ERenderAPI::None:
				return nullptr;
			case ERenderAPI::OpenGL:
				return new OpenGLIndexBuffer(Indices, Count);
			default:
				return nullptr;
		}
	}
}