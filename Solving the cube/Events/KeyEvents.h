#pragma once

#include "Event.h"

class KeyPressedEvent: public Event {
	
private:

	unsigned char key_;

public:

	KeyPressedEvent(unsigned char key): key_(key) {

	}

	EVENT_CLASS_TYPE(KEY_PRESSED)

	unsigned int getCategories() const override {
		return EventCategory::KEYBOARD | EventCategory::INPUT_M;
	}

	unsigned char key() {
		return key_;
	}
};