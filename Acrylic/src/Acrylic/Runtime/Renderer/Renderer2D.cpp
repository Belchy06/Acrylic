#include "acpch.h"
#include "Renderer2D.h"

#include "Core/Instrumentation.h"
#include "Renderer/Renderer.h"

#include <glm/ext/matrix_transform.hpp>

namespace Acrylic
{
	void Renderer2D::Init()
	{
		AC_PROFILE_FUNCTION()

		RendererBase::Init();

		Data = new Renderer2DStorage();

		// clang-format off
		float Vertices[4 * 5] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f, 
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f,
		};
		// clang-format on

		TSharedPtr<IVertexBuffer> VertexBuffer = IVertexBuffer::Create(Vertices, sizeof(Vertices) / sizeof(float));
		// clang-format off
		VertexBuffer->SetLayout({ 
			{ EDataType::Float3, "a_Position" },
			{ EDataType::Float2, "a_TexCoord" } 
		});
		// clang-format on

		// clang-format off
		uint32_t Indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on
		TSharedPtr<IIndexBuffer> IndexBuffer = IIndexBuffer::Create(Indices, sizeof(Indices) / sizeof(uint32_t));

		Data->VertexArray = IVertexArray::Create();
		Data->VertexArray->AddVertexBuffer(VertexBuffer);
		Data->VertexArray->SetIndexBuffer(IndexBuffer);

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
	}

	void Renderer2D::EndScene()
	{
		AC_PROFILE_FUNCTION()
	}

	void Renderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Colour)
	{
		AC_PROFILE_FUNCTION()

		DrawQuad(glm::vec3(Position, 0.f), Size, Colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Colour)
	{
		AC_PROFILE_FUNCTION()

		Data->Shader->UploadUniformFloat4("u_Color", Colour);
		Data->Shader->UploadUniformFloat("u_TilingFactor", 1.f);
		Data->WhiteTexture->Bind();

		glm::mat4 Transform = glm::translate(glm::mat4(1.f), Position) * glm::scale(glm::mat4(1.f), glm::vec3(Size, 1.f));
		Data->Shader->UploadUniformMat4("u_Transform", Transform);

		Data->VertexArray->Bind();
		GCommandListExecutor->DrawIndexed(Data->VertexArray);
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