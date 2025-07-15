#pragma once

#include "Event.h"

namespace Juniper {

	struct WindowResizeEvent : public Event
	{
		int Width, Height;

		WindowResizeEvent(int width, int height) :
			Width(width), Height(height) {}

		static EventType GetStaticType() { return EventType::WindowResize; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "WindowResizeEvent"; }
	};

	struct WindowCloseEvent : public Event
	{
		WindowCloseEvent() {}

		static EventType GetStaticType() { return EventType::WindowClose; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "WindowCloseEvent"; }
	};

	struct WindowMoveEvent : public Event
	{
		int XPos, YPos;
		WindowMoveEvent(int xpos, int ypos) :
			XPos(xpos), YPos(ypos) {}

		static EventType GetStaticType() { return EventType::WindowMove; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "WindowMoveEvent"; }
	};

	struct WindowFocusEvent : public Event
	{
		WindowFocusEvent() {}

		static EventType GetStaticType() { return EventType::WindowFocus; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "WindowFocusEvent"; }
	};

	struct WindowFocusLostEvent : public Event
	{
		WindowFocusLostEvent() {}

		static EventType GetStaticType() { return EventType::WindowFocusLost; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "WindowFocusLostEvent"; }
	};

}