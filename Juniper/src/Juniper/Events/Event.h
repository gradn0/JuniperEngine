#pragma once

namespace Juniper {

	enum class EventType
	{
		None = 0,
		KeyPress, KeyRelease, Char,
		MouseMove, MouseButtonPress, MouseButtonRelease, MouseScroll,
		WindowResize, WindowClose, WindowMove, WindowFocus, WindowFocusLost
	};

	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;

		bool IsHandled = false;
	};

	/**
	* Casts the event to it's derived type via Dispatch() which calls the provided handler.
	* Avoids tedious type checking and branching in the parent function.
	*/
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e) : m_Event(e) {}

		template<typename T, typename F>
		bool Dispatch(const F& handler)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.IsHandled = handler(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

}