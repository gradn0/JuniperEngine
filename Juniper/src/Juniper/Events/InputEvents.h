#pragma once

#include "Event.h"

namespace Juniper {

	struct KeyPressEvent : public Event
	{
		int Key, RepeatCount;

		KeyPressEvent(int key, int repeatCount) :
			Key(key), RepeatCount(repeatCount) {}

		static EventType GetStaticType() { return EventType::KeyPress; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyPressEvent"; }
	};

	struct KeyReleaseEvent : public Event
	{
		int Key;

		KeyReleaseEvent(int key) :
			Key(key) {}

		static EventType GetStaticType() { return EventType::KeyRelease; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyReleaseEvent"; }
	};

	struct CharEvent : public Event
	{
		int Char;

		CharEvent(unsigned int c) :
			Char(c) {
		}

		static EventType GetStaticType() { return EventType::Char; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "CharEvent"; }
	};

	struct MouseMoveEvent : public Event
	{
		double X, Y;

		MouseMoveEvent(double x, double y) :
			X(x), Y(y) {}

		static EventType GetStaticType() { return EventType::MouseMove; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseMoveEvent"; }
	};

	struct MouseButtonPressEvent : public Event
	{
		int Button;

		MouseButtonPressEvent(int button) :
			Button(button) {}

		static EventType GetStaticType() { return EventType::MouseButtonPress; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseButtonPressEvent"; }
	};

	struct MouseButtonReleaseEvent : public Event
	{
		int Button;

		MouseButtonReleaseEvent(int button) :
			Button(button) {}

		static EventType GetStaticType() { return EventType::MouseButtonRelease; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseButtonReleaseEvent"; }
	};

	struct MouseScrollEvent : public Event
	{
		double XOffset, YOffset;

		MouseScrollEvent(double xoffset, double yoffset) :
			XOffset(xoffset), YOffset(yoffset) {}

		static EventType GetStaticType() { return EventType::MouseScroll; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseScrollEvent"; }
	};

}