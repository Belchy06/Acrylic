#include "acpch.h"
#include "OpenGLExecutor.h"

#include <glad/glad.h>

namespace Acrylic
{
	OpenGLExecutor::OpenGLExecutor()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLExecutor::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLExecutor::SetClearColour(const glm::vec4& Colour)
	{
		glClearColor(Colour.r, Colour.g, Colour.b, Colour.a);
	}

	void OpenGLExecutor::DrawIndexed(const TSharedPtr<IVertexArray>& VertexArray)
	{
		glDrawElements(GL_TRIANGLES, VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLExecutor::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		glViewport(X, Y, Width, Height);
	}
}
