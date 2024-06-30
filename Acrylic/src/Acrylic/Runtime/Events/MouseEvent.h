#pragma once

#include "Events/Event.h"

namespace Acrylic
{
	class ACRYLIC_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float X, float Y)
			: DeltaX(X)
			, DeltaY(Y)
		{
		}

		FORCEINLINE float GetDeltaX() { return DeltaX; }
		FORCEINLINE float GetDeltaY() { return DeltaY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << DeltaX << ", " << DeltaY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EEventCategory::Mouse | EEventCategory::Input)

	private:
		float DeltaX;
		float DeltaY;
	};

	class ACRYLIC_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float X, float Y)
			: X(X)
			, Y(Y)
		{
		}

		FORCEINLINE float GetX() { return X; }
		FORCEINLINE float GetY() { return Y; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << X << ", " << Y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EEventCategory::Mouse | EEventCategory::Input)

	private:
		float X;
		float Y;
	};

	class ACRYLIC_API MouseButtonEvent : public Event
	{
	public:
		FORCEINLINE int GetMouseButton() { return Button; }

		EVENT_CLASS_CATEGORY(EEventCategory::Mouse | EEventCategory::Input)

	protected:
		MouseButtonEvent(int Button)
			: Button(Button)
		{
		}

	protected:
		int Button;
	};

	class ACRYLIC_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int Button)
			: MouseButtonEvent(Button)
		{
		}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class ACRYLIC_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int Button)
			: MouseButtonEvent(Button)
		{
		}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}