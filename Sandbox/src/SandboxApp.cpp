#include "Acrylic.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSandbox, Log);
DEFINE_LOG_CATEGORY(LogSandbox);

class ExampleLayer : public Acrylic::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
	}

	virtual void OnUpdate() override
	{
		// AC_LOG(LogSandbox, Log, "ExampleLayer::OnUpdate");
	}

	virtual void OnEvent(Acrylic::Event& e) override
	{
		AC_LOG(LogSandbox, Log, "ExampleLayer::OnEvent {0}", e.ToString());
	}
};

class Sandbox : public Acrylic::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	virtual ~Sandbox() = default;
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}