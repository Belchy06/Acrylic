#include "acpch.h"
#include "Shader.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLShader.h"

namespace Acrylic
{
	IShader* IShader::Create(const std::string& VertexSrc, const std::string& FragmentSrc)
	{
		switch (Renderer::GetRenderInterface())
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return new OpenGLShader(VertexSrc, FragmentSrc);
			default:
				return nullptr;
		}
	}
} // namespace Acrylic