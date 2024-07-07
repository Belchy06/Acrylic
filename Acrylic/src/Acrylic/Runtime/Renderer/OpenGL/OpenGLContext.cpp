#include "acpch.h"
#include "OpenGLContext.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogOpenGLContext);

	OpenGLContext::OpenGLContext(GLFWwindow* WindowHandle)
		: WindowHandle(WindowHandle)
	{
		AC_ASSERT(WindowHandle);
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(WindowHandle);

		int Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AC_ASSERT(Status);

		AC_LOG(LogOpenGLContext, Log, "OpenGL Info");
		AC_LOG(LogOpenGLContext, Log, "\tVendor: {0}", glGetString(GL_VENDOR));
		AC_LOG(LogOpenGLContext, Log, "\tRenderer: {0}", glGetString(GL_RENDERER));
		AC_LOG(LogOpenGLContext, Log, "\tVersion: {0}", glGetString(GL_VERSION));

		int Major, Minor;
		glGetIntegerv(GL_MAJOR_VERSION, &Major);
		glGetIntegerv(GL_MINOR_VERSION, &Minor);
		AC_ASSERT(Major > 4 || Major == 4 && Minor >= 5);
	}

	void OpenGLContext::SwapBuffers()
	{
		AC_ASSERT(WindowHandle);
		glfwSwapBuffers(WindowHandle);
	}
} // namespace Acrylic