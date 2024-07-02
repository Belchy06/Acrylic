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

		std::vector<Layer*>::iterator begin() { return Layers.begin(); }
		std::vector<Layer*>::iterator end() { return Layers.end(); }

	private:
		std::vector<Layer*> Layers;
		uint32_t			LayerInsertIndex = 0;
	};
} // namespace Acrylic
