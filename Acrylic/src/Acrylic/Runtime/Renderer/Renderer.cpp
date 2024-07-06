#include "acpch.h"
#include "Renderer.h"

#include "CommandList.h"

#include "Renderer/OpenGL/OpenGLExecutor.h"

namespace Acrylic
{
	void Renderer::Init()
	{
		if (GCommandListExecutor)
		{
			// Renderer has already been initialized
			return;
		}

		// TODO: parse command line for rendering type
		API = ERenderInterface::OpenGL;

		switch (Renderer::GetRenderInterface())
		{
			case ERenderInterface::None:
				break;
			case ERenderInterface::OpenGL:
				GCommandListExecutor = new OpenGLExecutor();
				break;
			default:
				break;
		}
	}

	void Renderer::OnWindowResize(uint32_t Width, uint32_t Height)
	{
		GCommandListExecutor->SetViewport(0, 0, Width, Height);
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
		Shader->UploadUniformMat4("u_ViewProjection", Data->ViewProjectionMatrix);
		Shader->UploadUniformMat4("u_Transform", TransformationMatrix);

		VertexArray->Bind();
		GCommandListExecutor->DrawIndexed(VertexArray);
	}
} // namespace Acrylic