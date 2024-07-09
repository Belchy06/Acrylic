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

		RendererStats = new Stats();

		Data = new Renderer2DStorage();
		Data->QuadVertexArray = IVertexArray::Create();

		Data->QuadVertexBuffer = IVertexBuffer::Create(MAXVERTICES * sizeof(QuadVertex));
		// clang-format off
		Data->QuadVertexBuffer->SetLayout({ 
			{ EDataType::Float3, "a_Position" },
			{ EDataType::Float4, "a_Color" },
			{ EDataType::Float2, "a_TexCoord" },
			{ EDataType::Float, "a_TexIndex" },
			{ EDataType::Float, "a_TilingFactor" }
		});
		// clang-format on
		Data->QuadVertexArray->AddVertexBuffer(Data->QuadVertexBuffer);

		Data->QuadVertexPtr = new QuadVertex[MAXVERTICES];

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

		Data->QuadIndexBuffer = IIndexBuffer::Create(QuadIndices, MAXINDICES);
		Data->QuadVertexArray->SetIndexBuffer(Data->QuadIndexBuffer);
		delete[] QuadIndices;

		uint32_t		  WhiteTextureData = 0xFFFFFFFF;
		CreateTextureDesc Desc = { 1, 1, EPixelFormat::RGB8, new UploadArrayBulkData(&WhiteTextureData, 4) };
		Data->Textures[0] = Texture2D::Create(Desc);

		int Samplers[MAXTEXTURESLOTS];
		for (uint32_t i = 0; i < MAXTEXTURESLOTS; i++)
		{
			Samplers[i] = i;
		}

		Data->QuadShader = IShader::Create("assets/shaders/Texture.glsl");
		Data->QuadShader->Bind();
		Data->QuadShader->UploadUniformIntArray("u_Texture", Samplers, MAXTEXTURESLOTS);

		// clang-format off
		Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Data->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		Data->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		// clang-format on
	}

	void Renderer2D::Shutdown()
	{
		AC_PROFILE_FUNCTION()

		delete RendererStats;
		delete Data;
	}

	void Renderer2D::StartBatch()
	{
		Data->QuadIndexCount = 0;
		Data->QuadVertexPtrEnd = Data->QuadVertexPtr;

		Data->TexturesIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::BeginScene(TSharedPtr<ICamera> Camera)
	{
		AC_PROFILE_FUNCTION()

		Data->QuadShader->Bind();
		Data->QuadShader->UploadUniformMat4("u_ViewProjection", Camera->GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::Flush()
	{
		if (Data->QuadIndexCount > 0)
		{
			Data->QuadShader->Bind();

			uint32_t DataNum = PTRDIFF_TO_UINT32(Data->QuadVertexPtrEnd - Data->QuadVertexPtr);
			Data->QuadVertexBuffer->SetData(Data->QuadVertexPtr, DataNum * sizeof(QuadVertex));

			for (uint32_t i = 0; i < Data->TexturesIndex; i++)
			{
				Data->Textures[i]->Bind(i);
			}

			GCommandListExecutor->DrawIndexed(Data->QuadVertexArray, Data->QuadIndexCount);

			RendererStats->DrawCalls++;
		}
	}

	void Renderer2D::EndScene()
	{
		AC_PROFILE_FUNCTION()

		Flush();
	}

	void Renderer2D::DrawQuad(const DrawQuadProps& Props)
	{
		AC_PROFILE_FUNCTION()

		if (Data->QuadIndexCount >= MAXINDICES)
		{
			NextBatch();
		}

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
				if (Data->TexturesIndex >= MAXTEXTURESLOTS)
				{
					NextBatch();
				}

				TextureIndex = Data->TexturesIndex++;
				Data->Textures[TextureIndex] = Props.Texture;
			}
		}

		glm::mat4 Transform =
			glm::translate(glm::mat4(1.f), Props.Position) *				 // translate
			glm::rotate(glm::mat4(1.f), Props.Rotation, { 0.f, 0.f, 1.f }) * // rotate
			glm::scale(glm::mat4(1.f), glm::vec3(Props.Size, 1.f));			 // scale

		constexpr size_t QuadVertexCount = 4;

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			Data->QuadVertexPtrEnd->Position = Transform * Data->QuadVertexPositions[i];
			Data->QuadVertexPtrEnd->Colour = Props.Colour;
			Data->QuadVertexPtrEnd->TexCoord = Props.Texture ? Props.Texture->GetTexCoords()[i] : glm::vec2(0.f);
			Data->QuadVertexPtrEnd->TexIndex = (float)TextureIndex;
			Data->QuadVertexPtrEnd->TilingFactor = Props.TilingFactor;
			Data->QuadVertexPtrEnd++;
		}

		Data->QuadIndexCount += 6;

		RendererStats->QuadCount++;
	}
} // namespace Acrylic