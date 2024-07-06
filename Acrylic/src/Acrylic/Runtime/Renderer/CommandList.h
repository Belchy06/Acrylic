#pragma once

#include "Renderer/IRenderer.h"

#include "Renderer/OpenGL/OpenGLRenderer.h"

namespace Acrylic
{
	class ACRYLIC_API CommandList
	{
	public:
		static inline void Clear()
		{
			GetRenderer()->Clear();
		}

		static inline void SetClearColour(const glm::vec4& Colour)
		{
			GetRenderer()->SetClearColour(Colour);
		}

		static inline void DrawIndexed(const TSharedPtr<IVertexArray>& VertexArray)
		{
			GetRenderer()->DrawIndexed(VertexArray);
		}

		static IRenderer* GetRenderer() { return Renderer; } 

	private:
		inline static IRenderer* Renderer = new OpenGLRenderer();
	};
}