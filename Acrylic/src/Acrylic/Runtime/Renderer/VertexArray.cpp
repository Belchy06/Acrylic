#include "acpch.h"
#include "VertexArray.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLVertexArray.h"

namespace Acrylic
{
	TSharedPtr<IVertexArray> IVertexArray::Create()
	{
		switch (Renderer::GetRenderInterface())
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLVertexArray>();
			default:
				return nullptr;
		}
	}
}