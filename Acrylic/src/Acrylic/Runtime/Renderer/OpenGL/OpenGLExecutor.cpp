#include "acpch.h"
#include "OpenGLExecutor.h"

#include "Core/Instrumentation.h"

#include <glad/glad.h>

namespace Acrylic
{
	OpenGLExecutor::OpenGLExecutor()
	{
		AC_PROFILE_FUNCTION()

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLExecutor::Clear()
	{
		AC_PROFILE_FUNCTION()

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLExecutor::SetClearColour(const glm::vec4& Colour)
	{
		AC_PROFILE_FUNCTION()

		glClearColor(Colour.r, Colour.g, Colour.b, Colour.a);
	}

	void OpenGLExecutor::DrawIndexed(const TSharedPtr<IVertexArray>& VertexArray, uint32_t IndexCount)
	{
		AC_PROFILE_FUNCTION()

		glDrawElements(GL_TRIANGLES, IndexCount > 0 ? IndexCount : VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLExecutor::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		AC_PROFILE_FUNCTION()

		glViewport(X, Y, Width, Height);
	}
}
