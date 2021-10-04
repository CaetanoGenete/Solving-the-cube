#include "Camera.h"

#include "glm\gtc\matrix_transform.hpp"

#define NORMALISED_SCREEN_SIZE 2.0

Camera::Camera(const Window& window) :
	screen_width(&window.width()), screen_height(&window.height())
{}

void Camera::moveTo(float x, float y, float z) {
	glm::vec3 translate_by = glm::vec3(x, y, z) - position_;

	moveBy(translate_by.x, translate_by.y, translate_by.z);
}

void Camera::moveBy(float x, float y, float z) {
	glm::vec3 translate_by = glm::vec3(x, y, z);

	view_matrix_ = glm::translate(view_matrix_, translate_by);

	position_ += translate_by;
}

void Camera::lookAt(float x, float y, float z) {
	glm::vec3 look_at = glm::vec3(x, y, z);

	view_matrix_ = glm::lookAt(position_, look_at, glm::vec3(0, 1, 0));	
}

void Camera::pixelCoordToScreenCoord(double& x, double& y) {
	x = (NORMALISED_SCREEN_SIZE * x) / *screen_width - 1.0;
	y = 1.0 - (NORMALISED_SCREEN_SIZE * y) / *screen_height;
}

glm::vec4 Camera::screenToEyeRay(double& x, double &y) {
	pixelCoordToScreenCoord(x, y);

	glm::vec4 clipCoord = glm::vec4(x, y, -1.0, 1.0);
	glm::vec4 eye_ray = glm::inverse(projection_matrix_) * clipCoord;//

	return glm::vec4(eye_ray.x, eye_ray.y, -1.0, 0.0);
}

glm::vec3 Camera::eyeRayToWorldRay(glm::vec4& ray) {
	glm::vec4 world_ray = glm::inverse(view_matrix_) * ray;

	return glm::normalize(glm::vec3(world_ray.x, world_ray.y, world_ray.z));
}

const glm::vec3& Camera::position() const {
	return position_;
}

const glm::mat4 Camera::getPV() const {
	return projection_matrix_ * view_matrix_;
}



