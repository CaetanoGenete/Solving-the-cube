#pragma once

#include "glm\glm.hpp"
#include "Window\Window.h"

class Camera {
private:

	glm::vec3 position_;
	glm::vec3 forward_;
	glm::vec3 right_;
	glm::vec3 up_;

	glm::mat4 view_matrix_;
	glm::mat4 projection_matrix_;

	unsigned int* screen_width;
	unsigned int* screen_height;

public:

	Camera():
		position_(),
		forward_(),
		right_(),
		up_(),

		view_matrix_(),
		projection_matrix_(),

		screen_width(nullptr),
		screen_height(nullptr)
	{};
	Camera(const Window& window);

	void moveTo(float x, float y, float z);
	void moveBy(float x, float y, float z);

	void lookAt(float x, float y, float z);

	//transforms a coordinate (x, y) where 0 <= x < screen_width and 0 <= y < screen_height to -1 < x, y < 1 
	void pixelCoordToScreenCoord(double& x, double& y);

	//Converts from screen coordinate to a ray perpendicular to the xy-plane
	glm::vec4 screenToEyeRay(double& x, double &y);

	//Converts from eye ray to a ray that points away from the camera
	glm::vec3 eyeRayToWorldRay(glm::vec4& ray);

	inline void setProjection(const glm::mat4& projection) {
		projection_matrix_ = projection;
	}

	const glm::vec3& position() const;

	//gets camera transformation matrix
	const glm::mat4 getPV() const;

};