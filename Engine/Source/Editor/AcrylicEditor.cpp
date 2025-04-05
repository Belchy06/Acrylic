#include "ApplicationCore/Application.h"
#include "Logging/Logging.h"

class AcrylicEditor : public Application
{
public:
	AcrylicEditor() = default;
	~AcrylicEditor() = default;

	virtual void Run() override
	{
		AC_LOG(LogTemp, "App running: [{}] [{}]", "Hello", "World");
	}
};

Application* CreateApplication()
{
	return new AcrylicEditor();
}
