#pragma once

#include "Layers/Layer.h"

namespace Acrylic
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Acrylic::Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Acrylic::Event& e) override;

	private:
		bool bViewportFocused;
		bool bViewportHovered;
	};
} // namespace Acrylic
