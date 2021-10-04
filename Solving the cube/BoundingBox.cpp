#include "BoundingBox.h"

OBoundingBox::OBoundingBox(glm::vec3 min, glm::vec3 max) :
	min_(min), max_(max), model_matrix_(glm::mat4(1.0f))
{}

inline glm::vec2 planeCollision(glm::vec3 axis, glm::vec3 delta, glm::vec3 ray_dir , float min_bound, float max_bound) {

	float pos = glm::dot(axis, delta);
	float dir = glm::dot(axis, ray_dir);

	float t_min, t_max;

	if (dir >= 0) {
		t_min = (pos + min_bound);
		t_max = (pos + max_bound);
	}
	else {
		t_min = (pos + max_bound);
		t_max = (pos + min_bound);
	}
	
	return {t_min / dir, t_max / dir}; // return t0 and t1
}

bool OBoundingBox::rayIntersection(const glm::vec3 origin, const glm::vec3 direction) {

	glm::vec3 OBB_world_position(model_matrix_[3].x, model_matrix_[3].y, model_matrix_[3].z);
	glm::vec3 delta_position = OBB_world_position - origin;

	glm::vec2 x_intersect = planeCollision(glm::vec3(model_matrix_[0].x, model_matrix_[0].y, model_matrix_[0].z), delta_position, direction, min_.x, max_.x);
	glm::vec2 y_intersect = planeCollision(glm::vec3(model_matrix_[1].x, model_matrix_[1].y, model_matrix_[1].z), delta_position, direction, min_.y, max_.y);

	if (x_intersect.x > y_intersect.y || y_intersect.x > x_intersect.y)
		return false;

	//swap intersections
	if (y_intersect.x > x_intersect.x) 
		x_intersect.x = y_intersect.x;
	if (y_intersect.y < x_intersect.y)
		x_intersect.y = y_intersect.y;

	glm::vec2 z_intersect = planeCollision(glm::vec3(model_matrix_[2].x, model_matrix_[2].y, model_matrix_[2].z), delta_position, direction, min_.z, max_.z);;

	if (x_intersect.x > z_intersect.y || z_intersect.x > x_intersect.y)
		return false;

	return true;
}

