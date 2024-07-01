#pragma once

#include "Core/Core.h"
#include "Events/Event.h"

namespace Acrylic
{
	class ACRYLIC_API Layer
	{
	public:
		Layer(const std::string& DebugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		FORCEINLINE const std::string& GetName() const { return DebugName;  }
		
	private:
		std::string DebugName;
	};
} // namespace Acrylic