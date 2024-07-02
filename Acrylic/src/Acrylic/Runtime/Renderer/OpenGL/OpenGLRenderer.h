#pragma once

#include "Renderer/IRenderer.h"

namespace Acrylic
{
	class OpenGLRenderer : public IRenderer
	{
	public:
		OpenGLRenderer() = default;

		virtual void Clear() override;
		virtual void SetClearColour(const glm::vec4& Colour) override;
		virtual void DrawIndexed(const std::shared_ptr<IVertexArray>& VertexArray) override;
	};
} // namespace Acrylic
