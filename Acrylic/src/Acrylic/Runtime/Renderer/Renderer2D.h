#pragma once

#include "Camera/Camera.h"
#include "Core/Core.h"
#include "Core/Memory.h"
#include "Renderer/Buffer.h"
#include "Renderer/RHI.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

// TODO: RendererCapabilities
#define MAXQUADS 10000
#define MAXVERTICES MAXQUADS * 4
#define MAXINDICES MAXQUADS * 6
#define MAXTEXTURESLOTS 32

namespace Acrylic
{
	class ACRYLIC_API Renderer2D
	{
	public:
		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
		};

	public:
		static void				  Init();
		static void				  Shutdown();
		static const Stats* const GetStats() { return RendererStats; }
		static void				  ResetStats()
		{
			RendererStats->DrawCalls = 0;
			RendererStats->QuadCount = 0;
		}

		static void BeginScene(TSharedPtr<ICamera> Camera);
		static void Flush();
		static void EndScene();

		// Primitives
		struct DrawQuadProps
		{
			glm::vec3			 Position = glm::vec3(0.f);
			glm::vec2			 Size = glm::vec2(1.f);
			float				 Rotation = 0.f; // Degrees
			glm::vec4			 Colour = glm::vec4(1.f);
			TSharedPtr<ITexture> Texture = nullptr;
			float				 TilingFactor = 1.f;
		};
		static void DrawQuad(const DrawQuadProps& Props);

	private:
		static void StartBatch();
		static void NextBatch();

	private:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Colour;
			glm::vec2 TexCoord;
			float	  TexIndex;
			float	  TilingFactor;
		};

		struct Renderer2DStorage
		{
			TArray<TSharedPtr<ITexture>, MAXTEXTURESLOTS> Textures;
			uint32_t									  TexturesIndex = 1; // 0 = white texture

			glm::vec4 QuadVertexPositions[4];

			// Quads
			TSharedPtr<IVertexArray>  QuadVertexArray;
			TSharedPtr<IIndexBuffer>  QuadIndexBuffer;
			TSharedPtr<IVertexBuffer> QuadVertexBuffer;
			TSharedPtr<IShader>		  QuadShader;
			uint32_t				  QuadIndexCount = 0;
			QuadVertex*				  QuadVertexPtr = nullptr;
			QuadVertex*				  QuadVertexPtrEnd = nullptr;
		};

		inline static Renderer2DStorage* Data;
		inline static Stats*			 RendererStats;
	};
} // namespace Acrylic
