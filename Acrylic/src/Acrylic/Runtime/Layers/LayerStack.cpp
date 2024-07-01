#include "acpch.h"
#include "LayerStack.h"

namespace Acrylic
{
	LayerStack::LayerStack()
	{
		LayerInsert = Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* l : Layers)
		{
			delete l;
		}
	}

	void LayerStack::PushLayer(Layer* InLayer)
	{
		LayerInsert = Layers.emplace(LayerInsert, InLayer);
		InLayer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* InOverlay)
	{
		Layers.emplace_back(InOverlay);
		InOverlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* InLayer)
	{
		std::vector<Layer*>::iterator It = std::find(Layers.begin(), Layers.end(), InLayer);
		if (It != Layers.end())
		{
			Layers.erase(It);
			LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* InOverlay)
	{
		std::vector<Layer*>::iterator It = std::find(Layers.begin(), Layers.end(), InOverlay);
		if (It != Layers.end())
		{
			Layers.erase(It);
		}
	}
}