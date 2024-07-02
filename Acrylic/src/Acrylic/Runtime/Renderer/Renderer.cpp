#include "acpch.h"
#include "Renderer.h"

#include "CommandList.h"

namespace Acrylic
{
	void Renderer::Init()
	{
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<IShader> Shader, const std::shared_ptr<IVertexArray>& VertexArray)
	{
		Shader->Bind();
		VertexArray->Bind();
		CommandList::DrawIndexed(VertexArray);
	}
}