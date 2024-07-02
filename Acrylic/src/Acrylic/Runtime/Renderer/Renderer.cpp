#include "acpch.h"
#include "Renderer.h"

#include "CommandList.h"

namespace Acrylic
{
	void Renderer::Init()
	{
	}

	void Renderer::BeginScene(std::shared_ptr<ICamera> Camera)
	{
		Data->ViewProjectionMatrix = Camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<IShader> Shader, const std::shared_ptr<IVertexArray>& VertexArray, const glm::mat4 TransformationMatrix)
	{
		Shader->Bind();
		Shader->UploadUniformMat4("u_ViewProjection", Data->ViewProjectionMatrix);
		Shader->UploadUniformMat4("u_Transform", TransformationMatrix);

		VertexArray->Bind();
		CommandList::DrawIndexed(VertexArray);
	}
}