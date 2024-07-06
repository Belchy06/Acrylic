#include "acpch.h"

#include "Shader.h"

#include "Renderer/Renderer.h"
#include "Renderer/OpenGL/OpenGLShader.h"

namespace Acrylic
{
	TSharedPtr<IShader> IShader::Create(const String& Path, const String& Name)
	{
		switch (Renderer::GetRenderInterface())
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLShader>(Path, Name);
			default:
				return nullptr;
		}
	}

	TSharedPtr<IShader> IShader::Create(const String& VertexSrc, const String& FragmentSrc, const String& Name)
	{
		switch (Renderer::GetRenderInterface())
		{
			case ERenderInterface::None:
				return nullptr;
			case ERenderInterface::OpenGL:
				return MakeShared<OpenGLShader>(VertexSrc, FragmentSrc, Name);
			default:
				return nullptr;
		}
	}

	void ShaderLibrary::Add(const TSharedPtr<IShader>& Shader)
	{
		const String& Name = Shader->GetName();
		AC_ASSERT(Shaders.find(Name) == Shaders.end());

		Shaders[Name] = Shader;
	}

	TSharedPtr<IShader> ShaderLibrary::Load(const String& Path, const String& Name)
	{
		TSharedPtr<IShader> Shader = IShader::Create(Path, Name);
		Add(Shader);

		return Shader;
	}

	TSharedPtr<IShader> ShaderLibrary::Load(const String& VertexSrc, const String& FragmentSrc, const String& Name)
	{
		TSharedPtr<IShader> Shader = IShader::Create(VertexSrc, FragmentSrc, Name);
		Add(Shader);

		return Shader;
	}

	TSharedPtr<IShader> ShaderLibrary::Get(const String& Name)
	{
		AC_ASSERT(Shaders.find(Name) != Shaders.end());
		return Shaders[Name];
	}
} // namespace Acrylic