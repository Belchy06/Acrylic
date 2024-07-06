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
	};
} // namespace Acrylic
