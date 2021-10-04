#pragma once

#include "CUtils.h"
#include <functional>

enum class EventType {

	NONE = 0,																	    // no events
	WINDOW_CLOSED, WINDOW_RESIZED, WINDOW_FOCUSED, WINDOW_LOST_FOCUS, WINDOW_MOVED, // window events
	MOUSE_PRESSED, MOUSE_RELEASED, MOUSE_SCROLLED, MOUSE_MOVED,						// mouse events
	KEY_PRESSED, KEY_RELEASED														// keyboard events
};

// enum class containing all event flags that will be used
enum EventCategory {

	NONE         = 0x00,
	INPUT_M      = 0x01,
	KEYBOARD     = 0x02,
	MOUSE        = 0x04,
	MOUSE_BUTTON = 0x08,
	WINDOW       = 0x10

};

#define EVENT_CLASS_TYPE(type)\
EventType getEventType() const override { \
	return EventType::##type; \
}\
\
static EventType GetStaticType() {\
	return EventType::##type;\
}\
\
utils::String getName() const override {\
return #type;\
}\

class Event {

	friend class EventDispatcher; // allows EventDispatcher class to utilise private variables

public:
	virtual EventType getEventType() const = 0;
	virtual unsigned int getCategories() const = 0;

	virtual utils::String getName() const = 0;

	virtual utils::String toString() { // Provides extra information about the event
		return getName();
	}

	bool isInCategory(const unsigned int category) { // checks to see if event is of a certain category
		return (getCategories() & category);
	}

protected:

	bool handled_ = false;
};

class EventDispatcher {

	template<typename T>
	using EventFunction = std::function<bool(T&)>; //using function template so it may be easily used as a parameter

	Event& current_event_; // The current event being handled
	unsigned int filter_;  // The categories that the dispatcher will handle 

public:
	
	EventDispatcher(Event& event): 
		current_event_(event), filter_(event.getCategories()) // sets filter to the events category to ensure by default it is always handled
	{}

	template<typename T>
	bool dispatch(EventFunction<T> function) {   //This function handles the events
		if (!current_event_.isInCategory(filter_)) // If not in the filter category do not handle
			return false;

		// checks to see that the parameters of the function are of the right type and that the event hasnt already been handled
		if (current_event_.getEventType() == T::GetStaticType() && !current_event_.handled_) { 
			current_event_.handled_ = function(*(T*)&current_event_); // handles the events

			return true;
		}

		return false;
	}

	void filterBy(unsigned int categories) { // setter for filter variable
		filter_ = categories;
	}
};