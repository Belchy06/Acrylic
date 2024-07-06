#pragma once

#include "Renderer/CommandList.h"

namespace Acrylic
{
	class OpenGLExecutor : public ICommandListExecutor
	{
	public:
		OpenGLExecutor();

		virtual void Clear() override;
		virtual void SetClearColour(const glm::vec4& Colour) override;
		virtual void DrawIndexed(const TSharedPtr<IVertexArray>& VertexArray) override;
		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) override;
	};
} // namespace Acrylic
