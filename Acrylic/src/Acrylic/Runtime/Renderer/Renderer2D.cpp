#include "acpch.h"
#include "Renderer2D.h"

#include "Core/Instrumentation.h"
#include "Renderer/Renderer.h"

#include <glm/ext/matrix_transform.hpp>

#define MAXQUADS 10000
#define MAXVERTICES MAXQUADS * 4
#define MAXINDICES MAXQUADS * 6

namespace Acrylic
{
	void Renderer2D::Init()
	{
		AC_PROFILE_FUNCTION()

		RendererBase::Init();

		Data = new Renderer2DStorage();
		Data->VertexArray = IVertexArray::Create();

		Data->VertexBuffer = IVertexBuffer::Create(MAXVERTICES / sizeof(QuadVertex));
		// clang-format off
		Data->VertexBuffer->SetLayout({ 
			{ EDataType::Float3, "a_Position" },
			{ EDataType::Float4, "a_Color" },
			{ EDataType::Float2, "a_TexCoord" } 
		});
		// clang-format on

		Data->QuadVertexBuffer = new QuadVertex[MAXVERTICES];

		uint32_t* QuadIndices = new uint32_t[MAXINDICES];
		uint32_t  Offset = 0;
		for (uint32_t i = 0; i < MAXINDICES;)
		{
			QuadIndices[i++] = Offset + 0;
			QuadIndices[i++] = Offset + 1;
			QuadIndices[i++] = Offset + 2;

			QuadIndices[i++] = Offset + 2;
			QuadIndices[i++] = Offset + 3;
			QuadIndices[i++] = Offset + 0;

			Offset += 4;
		}

		Data->IndexBuffer = IIndexBuffer::Create(QuadIndices, MAXINDICES);
		delete[] QuadIndices;

		Data->VertexArray->AddVertexBuffer(Data->VertexBuffer);
		Data->VertexArray->SetIndexBuffer(Data->IndexBuffer);

		uint32_t		  WhiteTextureData = 0xFFFFFFFF;
		CreateTextureDesc Desc = { 1, 1, EPixelFormat::RGB8, new UploadArrayBulkData(&WhiteTextureData, 4) };
		Data->WhiteTexture = Texture2D::Create(Desc);

		Data->Shader = IShader::Create("assets/shaders/Texture.glsl");
		Data->Shader->Bind();
		Data->Shader->UploadUniformInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		AC_PROFILE_FUNCTION()

		delete Data;
	}

	void Renderer2D::BeginScene(TSharedPtr<ICamera> Camera)
	{
		AC_PROFILE_FUNCTION()

		Data->Shader->Bind();
		Data->Shader->UploadUniformMat4("u_ViewProjection", Camera->GetViewProjectionMatrix());

		Data->QuadIndex = 0;
		Data->QuadVertexBufferPtr = Data->QuadVertexBuffer;
	}

	void Renderer2D::Flush()
	{
		GCommandListExecutor->DrawIndexed(Data->VertexArray, Data->QuadIndex);
	}

	void Renderer2D::EndScene()
	{
		AC_PROFILE_FUNCTION()

		uint32_t DataNum = PTRDIFF_TO_UINT32(Data->QuadVertexBufferPtr - Data->QuadVertexBuffer);
		Data->VertexBuffer->SetData(Data->QuadVertexBuffer, DataNum * sizeof(QuadVertex));

		Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Colour)
	{
		AC_PROFILE_FUNCTION()

		DrawQuad(glm::vec3(Position, 0.f), Size, Colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Colour)
	{
		AC_PROFILE_FUNCTION()

		glm::mat4 Transform = glm::translate(glm::mat4(1.f), Position) * glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));

		// Top Right
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Position, 1.f);
		Data->QuadVertexBufferPtr->Colour = Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		Data->QuadVertexBufferPtr++;

		// Bottom Right
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Position.x + Size.x, Position.y, 0.f, 1.f);
		Data->QuadVertexBufferPtr->Colour = Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		Data->QuadVertexBufferPtr++;

		// Bottom Left
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Position.x + Size.x, Position.y + Size.y, 0.f, 1.f);
		Data->QuadVertexBufferPtr->Colour = Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		Data->QuadVertexBufferPtr++;

		// Top Left
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Position.x, Position.y + Size.y, 0.f, 1.f);
		Data->QuadVertexBufferPtr->Colour = Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		Data->QuadVertexBufferPtr++;

		Data->QuadIndex += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const TSharedPtr<ITexture>& Texture, const glm::vec4& Tint)
	{
		AC_PROFILE_FUNCTION()

		DrawQuad(glm::vec3(Position, 0.f), Size, Texture, Tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const TSharedPtr<ITexture>& Texture, const glm::vec4& Tint)
	{
		AC_PROFILE_FUNCTION()

		Data->Shader->UploadUniformFloat4("u_Color", Tint);
		Data->Shader->UploadUniformFloat("u_TilingFactor", 1.f);
		Texture->Bind();

		glm::mat4 Transform = glm::translate(glm::mat4(1.f), Position) * glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		Data->Shader->UploadUniformMat4("u_Transform", Transform);

		Data->VertexArray->Bind();
		GCommandListExecutor->DrawIndexed(Data->VertexArray);
	}
} // namespace Acrylic