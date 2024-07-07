#include "acpch.h"
#include "Renderer.h"

#include "Core/Instrumentation.h"
#include "Renderer/CommandList.h"

#include "Renderer/OpenGL/OpenGLExecutor.h"

namespace Acrylic
{
	void RendererBase::Init()
	{
		if (GCommandListExecutor)
		{
			// Renderer has already been initialized
			return;
		}

		// TODO: parse command line for rendering type
		GRenderInterface = ERenderInterface::OpenGL;

		switch (GRenderInterface)
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

	void Renderer::Init()
	{
		AC_PROFILE_FUNCTION()

		RendererBase::Init();
	}

	void Renderer::OnWindowResize(uint32_t Width, uint32_t Height)
	{
		AC_PROFILE_FUNCTION()

		GCommandListExecutor->SetViewport(0, 0, Width, Height);
	}

	void Renderer::BeginScene(TSharedPtr<ICamera> Camera)
	{
		AC_PROFILE_FUNCTION()

		Data->ViewProjectionMatrix = Camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		AC_PROFILE_FUNCTION()
	}

	void Renderer::Submit(const TSharedPtr<IShader> Shader, const TSharedPtr<IVertexArray>& VertexArray, const glm::mat4 TransformationMatrix)
	{
		AC_PROFILE_FUNCTION()

		Shader->Bind();
		Shader->UploadUniformMat4("u_ViewProjection", Data->ViewProjectionMatrix);
		Shader->UploadUniformMat4("u_Transform", TransformationMatrix);

		VertexArray->Bind();
		GCommandListExecutor->DrawIndexed(VertexArray);
	}
} // namespace Acrylic