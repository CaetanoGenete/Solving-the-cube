#pragma once

#include "glm\glm.hpp"
#include <iostream>

//Oriented bounding box

class OBoundingBox {

private:

	glm::vec3 max_, min_;
	
public:

	glm::mat4 model_matrix_;

	OBoundingBox(): max_(), min_(), model_matrix_() {}
	OBoundingBox(glm::vec3 min, glm::vec3 max);

	bool rayIntersection(const glm::vec3 origin, const glm::vec3 direction);
};