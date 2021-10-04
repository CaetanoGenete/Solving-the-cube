#pragma once

#include "glm\glm.hpp"
#include "CubieLevel.h"

struct Preferences {

	//Virtual 3D cube colour settings

	glm::vec4 colour_scheme[NO_FACES] = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
	};

	glm::vec4 hidden_cube_colour = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

	//Virtual 3D cube dimension settings

	float sticker_padding = 0.2f;
	
	//Virtual 3D cube camera settings:
	
	float camera_distance = 12.0f;
	float camera_sensitivity = 0.0022f;

	//Virtual 3D cube rotate:

	float rotation_speed =5.0f;
};

static Preferences current_preference_settings;
