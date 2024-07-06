#pragma once

#include "Core/Core.h"
#include "Core/Containers/String.h"
#include "Core/Misc/EnumClassFlags.h"

namespace Acrylic
{
	enum class EEventType : uint8_t
	{
		None,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	ENUM_CLASS_FLAGS(EEventType)

	enum class EEventCategory : uint8_t
	{
		None = 0 << 0,
		Application = 1 << 0,
		Input = 1 << 1,
		Keyboard = 1 << 2,
		Mouse = 1 << 3
	};

	ENUM_CLASS_FLAGS(EEventCategory)

#define EVENT_CLASS_TYPE(Type)                       \
	static EEventType GetStaticType()                \
	{                                                \
		return EEventType::Type;                     \
	}                                                \
	virtual EEventType GetEventType() const override \
	{                                                \
		return GetStaticType();                      \
	}                                                \
	virtual const char* GetName() const override     \
	{                                                \
		return #Type;                                \
	}

#define EVENT_CLASS_CATEGORY(Category) \
	virtual EEventCategory GetCategoryFlags() const override \
	{                                                        \
		return Category;                                     \
	}

	class ACRYLIC_API Event
	{
		friend class EventDispatcher;

	public:
		virtual EEventType	GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual EEventCategory GetCategoryFlags() const = 0;
		virtual String ToString() const { return GetName(); }

		FORCEINLINE bool IsInCategory(EEventCategory Category)
		{
			return static_cast<uint8_t>(GetCategoryFlags() & Category) != 0;
		}

		FORCEINLINE bool IsHandled() 
		{
			return bHandled;
		}

		FORCEINLINE void SetHandled()
		{
			bHandled = true;
		}

	protected:
		bool bHandled = false;
	};

	class ACRYLIC_API EventDispatcher
	{
	public:
		EventDispatcher(Event& InEvent)
			: EventInternal(InEvent)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& Func)
		{
			if (EventInternal.GetEventType() == T::GetStaticType())
			{
				EventInternal.bHandled = Func(*(T*)&EventInternal);
				return true;
			}
			return false;
		}

	private:
		Event& EventInternal;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
} // namespace Acrylic
