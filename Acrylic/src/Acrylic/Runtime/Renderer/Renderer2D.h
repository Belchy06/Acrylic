#pragma once

#include "Camera/Camera.h"
#include "Core/Core.h"
#include "Core/Memory.h"
#include "Renderer/Buffer.h"
#include "Renderer/RHI.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

namespace Acrylic
{
	class ACRYLIC_API Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(TSharedPtr<ICamera> Camera);
		static void Flush();
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Colour);
		static void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Colour);
		static void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const TSharedPtr<ITexture>& Texture, const glm::vec4& Tint = glm::vec4(1.f));
		static void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const TSharedPtr<ITexture>& Texture, const glm::vec4& Tint = glm::vec4(1.f));

	private:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Colour;
			glm::vec2 TexCoord;
		};

		struct Renderer2DStorage
		{
			TSharedPtr<IVertexArray>  VertexArray;
			TSharedPtr<IVertexBuffer> VertexBuffer;
			TSharedPtr<IIndexBuffer>  IndexBuffer;
			TSharedPtr<IShader>		  Shader;
			TSharedPtr<Texture2D>	  WhiteTexture;

			uint32_t	QuadIndex = 0;
			QuadVertex* QuadVertexBuffer = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;
		};

		inline static Renderer2DStorage* Data;
	};
} // namespace Acrylic
