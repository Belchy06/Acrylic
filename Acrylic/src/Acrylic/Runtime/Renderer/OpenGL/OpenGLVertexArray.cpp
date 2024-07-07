#include "acpch.h"
#include "OpenGLVertexArray.h"

#include "Core/Instrumentation.h"

#include <glad/glad.h>

namespace Acrylic
{
	static GLenum ToOpenGLType(EDataType Type)
	{
		switch (Type)
		{
			case EDataType::Float:
			case EDataType::Float2:
			case EDataType::Float3:
			case EDataType::Float4:
			case EDataType::Mat3:
			case EDataType::Mat4:
				return GL_FLOAT;
			case EDataType::Int:
			case EDataType::Int2:
			case EDataType::Int3:
			case EDataType::Int4:
				return GL_INT;
			case EDataType::Bool:
				return GL_BOOL;
			case EDataType::None:
			default:
				AC_ASSERT(false);
				return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: RendererId(0)
	{
		AC_PROFILE_FUNCTION()

		glCreateVertexArrays(1, &RendererId);

	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		AC_PROFILE_FUNCTION()

		glDeleteVertexArrays(1, &RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		AC_PROFILE_FUNCTION()

		glBindVertexArray(RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		AC_PROFILE_FUNCTION()

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const TSharedPtr<IVertexBuffer>& VertexBuffer)
	{
		AC_PROFILE_FUNCTION()

		AC_ASSERT(VertexBuffer->GetLayout().GetElements().size() > 0)

		glBindVertexArray(RendererId);
		VertexBuffer->Bind();

		uint32_t Index = 0;
		for (const BufferElement& Element : VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(
				Index,
				Element.GetComponentCount(),
				ToOpenGLType(Element.Type),
				Element.bNormalized ? GL_TRUE : GL_FALSE,
				VertexBuffer->GetLayout().GetStride(),
				reinterpret_cast<const void*>(Element.Offset));
			Index++;
		}

		VertexBuffers.push_back(VertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const TSharedPtr<IIndexBuffer>& InIndexBuffer)
	{
		AC_PROFILE_FUNCTION()

		glBindVertexArray(RendererId);
		InIndexBuffer->Bind();

		IndexBuffer = InIndexBuffer;
	}
}
