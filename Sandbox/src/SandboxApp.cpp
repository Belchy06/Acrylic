#include "Sandbox2D.h"

#include "Acrylic.h"
#include "Acrylic/Runtime/Entrypoint.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSandbox, Log);
DEFINE_LOG_CATEGORY(LogSandbox);

class Sandbox : public Acrylic::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
	virtual ~Sandbox() = default;
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}