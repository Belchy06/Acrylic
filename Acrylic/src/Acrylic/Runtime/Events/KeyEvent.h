#pragma once

#include "Events/Event.h"

namespace Acrylic
{
	class ACRYLIC_API KeyEvent : public Event
	{
	public:
		FORCEINLINE int GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY(EEventCategory::Keyboard | EEventCategory::Input)
	protected:
		KeyEvent(int KeyCode)
			: KeyCode(KeyCode)
		{
		}

		int KeyCode;
	};

	class ACRYLIC_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int KeyCode, int RepeatCount)
			: KeyEvent(KeyCode)
			, RepeatCount(RepeatCount)
		{
		}

		FORCEINLINE int GetRepeatCount() { return RepeatCount;  }

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << KeyCode << "(" << RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int RepeatCount;
	};

	class ACRYLIC_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int KeyCode)
			: KeyEvent(KeyCode)
		{
		}

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class ACRYLIC_API KeyCharEvent : public KeyEvent
	{
	public:
		KeyCharEvent(int KeyCode)
			: KeyEvent(KeyCode)
		{
		}

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "KeyCharEvent: " << KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
} // namespace Acrylic
