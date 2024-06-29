#include "Acrylic.h"

class Sandbox : public Acrylic::Application
{
public:
	Sandbox() = default;
	virtual ~Sandbox() = default;


};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}