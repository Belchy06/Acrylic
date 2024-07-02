#include "acpch.h"
#include "VertexArray.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLVertexArray.h"

namespace Acrylic
{
	IVertexArray* IVertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case ERenderAPI::None:
				return nullptr;
			case ERenderAPI::OpenGL:
				return new OpenGLVertexArray();
			default:
				return nullptr;
		}
	}
}