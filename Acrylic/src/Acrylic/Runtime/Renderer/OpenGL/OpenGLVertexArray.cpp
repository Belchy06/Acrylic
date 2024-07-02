#include "acpch.h"
#include "OpenGLVertexArray.h"

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
		glCreateVertexArrays(1, &RendererId);

	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& VertexBuffer)
	{
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

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& InIndexBuffer)
	{
		glBindVertexArray(RendererId);
		InIndexBuffer->Bind();

		IndexBuffer = InIndexBuffer;
	}
}
