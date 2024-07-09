#include "Core/Logging.h"
#include "Entrypoint.h"
#include "Layers/EditorLayer.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEditor, Log);
DEFINE_LOG_CATEGORY(LogEditor);

namespace Acrylic
{
	class AcrylicEditor : public Application
	{
	public:
		AcrylicEditor()
			: Application("Acrylic Editor")
		{
			AC_LOG(LogEditor, Log, "Creating Editor application");

			PushLayer(new EditorLayer());
		}
		virtual ~AcrylicEditor() = default;
	};

	Application* CreateApplication()
	{
		return new AcrylicEditor();
	}
} // namespace Acrylic