#include "acpch.h"
#include "Renderer/CommandList.h"

#include "Renderer/OpenGL/OpenGLRenderer.h"

namespace Acrylic
{
	IRenderer* CommandList::Renderer = new OpenGLRenderer();
}