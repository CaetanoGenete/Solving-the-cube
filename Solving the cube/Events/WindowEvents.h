#pragma once

#include "Event.h"

class WindowClosedEvent: public Event {

public:

	WindowClosedEvent() {};

	EVENT_CLASS_TYPE(WINDOW_CLOSED)

	unsigned int getCategories() const override {
		return EventCategory::WINDOW | EventCategory::INPUT_M;
	}

};