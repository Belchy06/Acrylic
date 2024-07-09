#pragma once

#include "Layers/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Acrylic
{
	class ACRYLIC_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool bBlockEvents) { bBlockingEvents = bBlockEvents; }

	private:
		float DeltaTime = 0.f;

		bool bBlockingEvents = false;
	};
} // namespace Acrylic
