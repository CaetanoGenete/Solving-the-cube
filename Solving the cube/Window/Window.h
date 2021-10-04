#pragma once

#include "CUtils.h"
#include <functional>

#include "..\Events\Event.h"

struct GLFWwindow;

struct WindowProperties {

	utils::String title_; //The text that will appear in the header of the window

	unsigned int width_; //The width in pixels of the window
	unsigned int height_; //The height in pixels of the window

	bool v_sync_; //Whether the buffer swaps are synced with the refresh rate of the screen

	unsigned int* preference_data_; //Data on the colours of the window

	using EventCallback = std::function<void(Event&)>;
	EventCallback eventCallBackFunction_; // The function to call when an event occurs

	// constructor
	WindowProperties(const utils::String& title = "window un-named", unsigned int width = 1280, unsigned int height = 720) :
		title_(title), 
		width_(width), 
		height_(height),

		v_sync_(false),

		preference_data_(nullptr)
	{}


};

class Window {

private:
	using EventCallback = std::function<void(Event&)>;

	WindowProperties* properties_; // Pointer to properties class

	GLFWwindow* window_; // A GLFW window

public:

	Window(WindowProperties& properties); // Constructor

	~Window(); // Destructor

	void closeWindow(); // Function designed to terminate a window

	void onUpdate();  // Updates the frames of the window

	unsigned int& width() const; // Getter for the width
	unsigned int& height() const; // Getter for the height

	void setEventCallBack(const EventCallback& function);
	
	void initialise(); // Sets up properties of a window

	inline GLFWwindow* window() {
		return window_;
	}
};