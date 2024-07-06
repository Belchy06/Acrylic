#include "acpch.h"
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace Acrylic
{
	void OpenGLRenderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderer::SetClearColour(const glm::vec4& Colour)
	{
		glClearColor(Colour.r, Colour.g, Colour.b, Colour.a);
	}

	void OpenGLRenderer::DrawIndexed(const TSharedPtr<IVertexArray>& VertexArray)
	{
		glDrawElements(GL_TRIANGLES, VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
