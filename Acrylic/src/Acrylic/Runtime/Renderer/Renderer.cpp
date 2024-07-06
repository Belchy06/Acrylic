#include "acpch.h"
#include "Renderer.h"

#include "CommandList.h"

// TEMP
#include "Renderer/OpenGL/OpenGLShader.h"

namespace Acrylic
{
	void Renderer::Init()
	{
	}

	void Renderer::BeginScene(TSharedPtr<ICamera> Camera)
	{
		Data->ViewProjectionMatrix = Camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const TSharedPtr<IShader> Shader, const TSharedPtr<IVertexArray>& VertexArray, const glm::mat4 TransformationMatrix)
	{
		Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(Shader)->UploadUniformMat4("u_ViewProjection", Data->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(Shader)->UploadUniformMat4("u_Transform", TransformationMatrix);

		VertexArray->Bind();
		CommandList::DrawIndexed(VertexArray);
	}
}