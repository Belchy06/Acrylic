#pragma once

#include "Renderer/IRenderer.h"

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
		static IRenderer* Renderer;
	};
}