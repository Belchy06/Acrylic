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
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Colour);
		static void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Colour);
		static void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const TSharedPtr<ITexture>& Texture, const glm::vec4& Tint = glm::vec4(1.f));
		static void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const TSharedPtr<ITexture>& Texture, const glm::vec4& Tint = glm::vec4(1.f));

	private:
		struct Renderer2DStorage
		{
			TSharedPtr<IVertexArray> VertexArray;
			TSharedPtr<IShader>		 Shader;
			TSharedPtr<Texture2D>	 WhiteTexture;
		};

		inline static Renderer2DStorage* Data;
	};
} // namespace Acrylic
