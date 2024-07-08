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
		Data->VertexArray = IVertexArray::Create();

		Data->VertexBuffer = IVertexBuffer::Create(MAXVERTICES / sizeof(QuadVertex));
		// clang-format off
		Data->VertexBuffer->SetLayout({ 
			{ EDataType::Float3, "a_Position" },
			{ EDataType::Float4, "a_Color" },
			{ EDataType::Float2, "a_TexCoord" },
			{ EDataType::Float, "a_TexIndex" },
			{ EDataType::Float, "a_TilingFactor" }
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
		Data->Textures[0] = Texture2D::Create(Desc);

		int Samplers[MAXTEXTURESLOTS];
		for (uint32_t i = 0; i < MAXTEXTURESLOTS; i++)
		{
			Samplers[i] = i;
		}

		Data->Shader = IShader::Create("assets/shaders/Texture.glsl");
		Data->Shader->Bind();
		Data->Shader->UploadUniformIntArray("u_Texture", Samplers, MAXTEXTURESLOTS);
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

		Data->TexturesIndex = 1;
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < Data->TexturesIndex; i++)
		{
			Data->Textures[i]->Bind(i);
		}

		GCommandListExecutor->DrawIndexed(Data->VertexArray, Data->QuadIndex);
	}

	void Renderer2D::EndScene()
	{
		AC_PROFILE_FUNCTION()

		uint32_t DataNum = PTRDIFF_TO_UINT32(Data->QuadVertexBufferPtr - Data->QuadVertexBuffer);
		Data->VertexBuffer->SetData(Data->QuadVertexBuffer, DataNum * sizeof(QuadVertex));

		Flush();
	}

	void Renderer2D::DrawQuad(const DrawQuadProps& Props)
	{
		AC_PROFILE_FUNCTION()

		glm::mat4 Transform = glm::translate(glm::mat4(1.f), Props.Position) * glm::rotate(glm::mat4(1.f), glm::radians(Props.Rotation), { 0.f, 0.f, 1.f }) * glm::scale(glm::mat4(1.f), glm::vec3(Props.Size, 1.f));

		uint32_t TextureIndex = 0;
		if (Props.Texture != nullptr)
		{
			// User has specified a texture. Update textures array and texture index
			for (uint32_t i = 1; i < Data->TexturesIndex; i++)
			{
				if (Data->Textures[i]->GetID() == Props.Texture->GetID())
				{
					TextureIndex = i;
					break;
				}
			}

			if (TextureIndex == 0)
			{
				TextureIndex = Data->TexturesIndex++;
				Data->Textures[TextureIndex] = Props.Texture;
			}
		}

		// Top Right
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Props.Position, 1.f);
		Data->QuadVertexBufferPtr->Colour = Props.Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		Data->QuadVertexBufferPtr->TexIndex = TextureIndex;
		Data->QuadVertexBufferPtr->TilingFactor = Props.TilingFactor;
		Data->QuadVertexBufferPtr++;

		// Bottom Right
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Props.Position.x + Props.Size.x, Props.Position.y, 0.f, 1.f);
		Data->QuadVertexBufferPtr->Colour = Props.Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		Data->QuadVertexBufferPtr->TexIndex = TextureIndex;
		Data->QuadVertexBufferPtr->TilingFactor = Props.TilingFactor;
		Data->QuadVertexBufferPtr++;

		// Bottom Left
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Props.Position.x + Props.Size.x, Props.Position.y + Props.Size.y, 0.f, 1.f);
		Data->QuadVertexBufferPtr->Colour = Props.Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		Data->QuadVertexBufferPtr->TexIndex = TextureIndex;
		Data->QuadVertexBufferPtr->TilingFactor = Props.TilingFactor;
		Data->QuadVertexBufferPtr++;

		// Top Left
		Data->QuadVertexBufferPtr->Position = Transform * glm::vec4(Props.Position.x, Props.Position.y + Props.Size.y, 0.f, 1.f);
		Data->QuadVertexBufferPtr->Colour = Props.Colour;
		Data->QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		Data->QuadVertexBufferPtr->TexIndex = TextureIndex;
		Data->QuadVertexBufferPtr->TilingFactor = Props.TilingFactor;
		Data->QuadVertexBufferPtr++;

		Data->QuadIndex += 6;
	}
} // namespace Acrylic