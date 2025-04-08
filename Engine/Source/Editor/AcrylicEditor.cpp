#include "ApplicationCore/Application.h"
#include "Logging/Logging.h"
#include "Containers/AcrylicString.h"

#if WITH_EDITOR
class AcrylicEditor : public Application
{
public:
	AcrylicEditor() = default;
	~AcrylicEditor() = default;

	virtual void Run() override
	{
		AC_LOG(LogTemp, "App running: [{}] [{}]", "Hello", "World");

		TString<WIDECHAR> TestString = L"TestString";
		std::wcout << *TestString << std::endl;

		TString<WIDECHAR> OtherString = *TestString;
		std::wcout << *OtherString << std::endl;
	}
};

Application* CreateApplication()
{
	return new AcrylicEditor();
}
#endif