#pragma once

#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Events/Event.h"

namespace Acrylic
{
	class ACRYLIC_API Layer
	{
	public:
		Layer(const String& DebugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		FORCEINLINE const String& GetName() const { return DebugName;  }
		
	private:
		String DebugName;
	};
} // namespace Acrylic