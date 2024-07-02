#pragma once

#include "acpch.h"

#include "Core/Core.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Acrylic
{
	struct WindowProperties
	{
		std::string Title;
		uint32_t	Width;
		uint32_t	Height;

		WindowProperties(const std::string& Title = "Acrylic Engine", uint32_t Width = 1280, uint32_t Height = 720)
			: Title(Title)
			, Width(Width)
			, Height(Height)
		{
		}
	};

	class ACRYLIC_API IWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~IWindow() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& Callback) = 0;
		virtual void SetVSync(bool bEnabled) = 0;
		virtual bool IsVSync() = 0;

		virtual void* GetNativeWindow() const = 0;

		static IWindow* Create(const WindowProperties& Properties = WindowProperties());
	};
}