#pragma once

#include "Core/Defines.h"

class ACRYLIC_API Application
{
public:
	Application() = default;
	~Application() = default;

	virtual void Run();
};