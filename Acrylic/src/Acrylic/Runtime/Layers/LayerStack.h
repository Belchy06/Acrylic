#pragma once

#include "Core/Core.h"
#include "Layers/Layer.h"

namespace Acrylic
{
	class ACRYLIC_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* InLayer);
		void PushOverlay(Layer* InOverlay);
		void PopLayer(Layer* InLayer);
		void PopOverlay(Layer* InOverlay);

		TArray<Layer*>::iterator	  begin() { return Layers.begin(); }
		TArray<Layer*>::iterator end() { return Layers.end(); }

	private:
		TArray<Layer*> Layers;
		uint32_t			LayerInsertIndex = 0;
	};
} // namespace Acrylic
