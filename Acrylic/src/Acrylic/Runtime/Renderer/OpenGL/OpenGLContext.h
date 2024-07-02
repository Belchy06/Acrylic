#pragma once

#include "Core/Logging/Logging.h"
#include "Renderer/Context.h"

struct GLFWwindow;

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogOpenGLContext, Log)

	class OpenGLContext : public ICommandContext
	{
	public:
		OpenGLContext(GLFWwindow* WindowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* WindowHandle;
	};
} // namespace Acrylic::Rendering
