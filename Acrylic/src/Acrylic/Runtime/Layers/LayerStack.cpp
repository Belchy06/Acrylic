#include "acpch.h"
#include "LayerStack.h"

namespace Acrylic
{
	LayerStack::LayerStack()
	{
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
		Layers.emplace(Layers.begin() + LayerInsertIndex, InLayer);
		LayerInsertIndex++;
		InLayer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* InOverlay)
	{
		Layers.emplace_back(InOverlay);
		InOverlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* InLayer)
	{
		TArray<Layer*>::iterator It = std::find(Layers.begin(), Layers.end(), InLayer);
		if (It != Layers.end())
		{
			InLayer->OnAttach();
			Layers.erase(It);
			LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* InOverlay)
	{
		TArray<Layer*>::iterator It = std::find(Layers.begin(), Layers.end(), InOverlay);
		if (It != Layers.end())
		{
			InOverlay->OnDetach();
			Layers.erase(It);
		}
	}
}