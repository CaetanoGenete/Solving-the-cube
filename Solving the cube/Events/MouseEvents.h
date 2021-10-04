#pragma once

#include "Event.h"



class MouseButtonEvent : public Event {

protected:

	double x_, y_;
	int button_;

	MouseButtonEvent(double x, double y, int button) :
		x_(x), y_(y), button_(button)
	{};

public:

	unsigned int getCategories() const override {
		return EventCategory::MOUSE | EventCategory::INPUT_M;
	}

	int button() const {
		return button_;
	}

	double x() const {
		return x_;
	}

	double y() const {
		return y_;
	}
};

class MousePressedEvent : public MouseButtonEvent {

public:

	MousePressedEvent(double x, double y, int button) : MouseButtonEvent(x, y, button) {}

	EVENT_CLASS_TYPE(MOUSE_PRESSED)

};

class MouseReleasedEvent : public MouseButtonEvent {

public:

	MouseReleasedEvent(double x, double y, int button) : MouseButtonEvent(x, y, button) {}

	EVENT_CLASS_TYPE(MOUSE_RELEASED)

};

class MouseMovedEvent : public Event {

protected:

	double x_, y_;

public:

	MouseMovedEvent(double x, double y):
		x_(x), y_(y)
	{};

	EVENT_CLASS_TYPE(MOUSE_MOVED)

	unsigned int getCategories() const override {
		return EventCategory::MOUSE | EventCategory::INPUT_M;
	}
 
	double x() const {
		return x_;
	}

	double y() const {
		return y_;
	}

};