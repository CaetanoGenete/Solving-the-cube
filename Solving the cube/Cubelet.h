#pragma once

#include "glm\gtc\matrix_transform.hpp"
#include "glm\glm.hpp"

#include "CubieLevel.h"

#include "Graphics\Vertex.h"
#include "CUtils.h"

#define NO_FACES 6

struct CubeletFace {

private:
	utils::ArrayList<unsigned int> renderer_vertex_indicies_;
	utils::ArrayList<Vertex> verticies_;

public:

	CubeletFace() {}
	CubeletFace(const glm::mat4& transform, const glm::vec4& colour);

	void applyTransform(const glm::mat4& transform);
	void applyColour(const CubeletFace& colour, utils::ArrayList<Vertex>& verticies) const;

	void addToRender(utils::ArrayList<Vertex>& verticies, utils::ArrayList<unsigned int>& indices);

	inline void addQuad(const glm::mat4& transform, glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, glm::vec4 v4, glm::vec4 colour) {
		verticies_.add({ transform * v1, colour });
		verticies_.add({ transform * v2, colour });
		verticies_.add({ transform * v3, colour });
		verticies_.add({ transform * v4, colour });
	}
};


struct Cubelet {

private:

	utils::ArrayList<CubeletFace> faces_;
	utils::ArrayList<unsigned int> coloured_faces_;

	unsigned char coloured_flags_;

public:

	Cubelet(): coloured_flags_(0) {}
	Cubelet(const glm::vec3& translate);

	void applyTransform(const glm::mat4& transform);
	void changeColours(unsigned int orientation, const Cubelet& cubelet, utils::ArrayList<Vertex>& verticies) const;

	void addToRender(utils::ArrayList<Vertex>& verticies, utils::ArrayList<unsigned int>& indices);

	bool isFaceColoured(const Face& face) const;
};
