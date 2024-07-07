#include "acpch.h"
#include "OpenGLBuffer.h"

#include "Core/Instrumentation.h"

#include <glad/glad.h>

namespace Acrylic
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t Size)
		: RendererId(0)
	{
		AC_PROFILE_FUNCTION()

		glCreateBuffers(1, &RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, RendererId);
		glBufferData(GL_ARRAY_BUFFER, Size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* Vertices, uint32_t Count)
		: RendererId(0)
	{
		AC_PROFILE_FUNCTION()

		glCreateBuffers(1, &RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, RendererId);
		glBufferData(GL_ARRAY_BUFFER, Count * sizeof(float), Vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		AC_PROFILE_FUNCTION()

		glDeleteBuffers(1, &RendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		AC_PROFILE_FUNCTION()

		glBindBuffer(GL_ARRAY_BUFFER, RendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		AC_PROFILE_FUNCTION()

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* Data, uint32_t Size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, RendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, Size, Data);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* Indices, uint32_t Count)
		: RendererId(0)
		, Count(Count)
	{
		AC_PROFILE_FUNCTION()

		glCreateBuffers(1, &RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, RendererId);
		glBufferData(GL_ARRAY_BUFFER, Count * sizeof(uint32_t), Indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		AC_PROFILE_FUNCTION()

		glDeleteBuffers(1, &RendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		AC_PROFILE_FUNCTION()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		AC_PROFILE_FUNCTION()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
} // namespace Acrylic