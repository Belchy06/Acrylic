#pragma once

#include "Events/Event.h"

#include <sstream>

namespace Acrylic
{
	class ACRYLIC_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t Width, uint32_t Height)
			: Width(Width)
			, Height(Height)
		{
		}

		FORCEINLINE uint32_t GetWidth() { return Width; }
		FORCEINLINE uint32_t GetHeight() { return Height; }

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << Width << ", " << Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EEventCategory::Application)

	private:
		uint32_t Width, Height;
	};

	class ACRYLIC_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EEventCategory::Application)
	};

	class ACRYLIC_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "AppTickEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EEventCategory::Application)
	};

	class ACRYLIC_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "AppUpdateEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EEventCategory::Application)
	};

	class ACRYLIC_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "AppRenderEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EEventCategory::Application)
	};
} // namespace Acrylic