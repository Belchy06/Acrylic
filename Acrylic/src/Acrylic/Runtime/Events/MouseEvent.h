#pragma once

#include "Events/Event.h"

namespace Acrylic
{
	class ACRYLIC_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double X, double Y)
			: DeltaX(X)
			, DeltaY(Y)
		{
		}

		FORCEINLINE double GetDeltaX() { return DeltaX; }
		FORCEINLINE double GetDeltaY() { return DeltaY; }

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << DeltaX << ", " << DeltaY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EEventCategory::Mouse | EEventCategory::Input)

	private:
		double DeltaX;
		double DeltaY;
	};

	class ACRYLIC_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double X, double Y)
			: X(X)
			, Y(Y)
		{
		}

		FORCEINLINE double GetX() { return X; }
		FORCEINLINE double GetY() { return Y; }

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << X << ", " << Y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EEventCategory::Mouse | EEventCategory::Input)

	private:
		double X;
		double Y;
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

		virtual String ToString() const override
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

		virtual String ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}