#include "acpch.h"
#include "VertexArray.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLVertexArray.h"

namespace Acrylic
{
	IVertexArray* IVertexArray::Create()
	{
		switch (Renderer::GetRenderInterface())
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return new OpenGLVertexArray();
			default:
				return nullptr;
		}
	}
}