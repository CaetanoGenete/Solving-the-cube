#include "Window.h"

#include "GLFW\glfw3.h"
#include "../Events/WindowEvents.h"
#include "../Events/MouseEvents.h"
#include "../Events/KeyEvents.h"

static bool GLFW_Initialised = false; // A static boolean value to check where GLFW has been initialised

Window::Window(WindowProperties& properties):
	properties_(&properties)
{
}

Window::~Window() {
	closeWindow(); //When the deletion of the window is called, make sure to close the window
}

void Window::closeWindow() {
	glfwDestroyWindow(window_); //deletes the GLFW window
}

void Window::onUpdate() {
	//glfwMakeContextCurrent(window_);

	glfwPollEvents(); // Listen for events such as mouse clicks and resizes
	glfwSwapBuffers(window_);
}

unsigned int& Window::width() const {
	return properties_->width_;
}

unsigned int& Window::height() const {
	return properties_->height_;
}

void Window::setEventCallBack(const EventCallback & function) {
	properties_->eventCallBackFunction_ = function;
}

void Window::initialise()
{
	if (!GLFW_Initialised) { //If GLFW is not inialised, nothing will work so it must be intialised

		int success = glfwInit();
		GLFW_Initialised = true;
	}

	window_ = glfwCreateWindow(properties_->width_, properties_->height_, properties_->title_.getData(), nullptr, nullptr); // Create the window
	glfwMakeContextCurrent(window_); // Make it the primary window that OpenGL draws onto
	glfwSetWindowUserPointer(window_, properties_); // EDITED: stores the properties object in the window
	glfwSwapInterval(properties_->v_sync_); // set vsync


	glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
		WindowClosedEvent event;

		WindowProperties properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		properties.eventCallBackFunction_(event);
	});

	glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double x, double y) {
		MouseMovedEvent event(x, y);

		WindowProperties properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		properties.eventCallBackFunction_(event);
	});


	glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

		WindowProperties properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
		
		switch (action) {
		case GLFW_PRESS:
			KeyPressedEvent event(key);
			properties.eventCallBackFunction_(event);

			break;
		}
	});
}


