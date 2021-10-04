#include "Cubelet.h"

#include "Preferences.h"

#define EDGES_ON_QUAD 4

const glm::vec3 z_axis_unit = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 x_axis_unit = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 y_axis_unit = glm::vec3(0.0f, 1.0f, 0.0f);

//All 90 degree rotation transformations around the z axis
const glm::mat4 z_axis_90_degree_rotations[360 / 90] = {
	glm::rotate(glm::mat4(1.0f), glm::radians(0.0f)  , z_axis_unit),
	glm::rotate(glm::mat4(1.0f), glm::radians(90.0f) , z_axis_unit),
	glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), z_axis_unit),
	glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), z_axis_unit)
};

//Order of verticies in a quadrilateral
const unsigned int quad_index_distribution[EDGES_ON_QUAD + 2] = {
	0, 1, 2, 2, 3, 0
};

const unsigned int face_indices = 4;

CubeletFace::CubeletFace(const glm::mat4& transform, const glm::vec4& colour) {
	float padding = current_preference_settings.sticker_padding;

	addQuad(
		transform,

		glm::vec4(-1.0f, 1.0f, 1.0f + padding, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f + padding, 1.0f),
		glm::vec4(1.0f, -1.0f, 1.0f + padding, 1.0f),
		glm::vec4(-1.0f, -1.0f, 1.0f + padding, 1.0f),

		colour
	);

	for (unsigned int j = 0; j < EDGES_ON_QUAD; j++) {
		addQuad(
			transform * z_axis_90_degree_rotations[j],

			glm::vec4(-1.0f - padding, 1.0f + padding, 1.0f + padding, 1.0f),
			glm::vec4( 1.0f + padding, 1.0f + padding, 1.0f + padding, 1.0f),
			glm::vec4( 1.0f, 1.0f, 1.0f + padding, 1.0f),
			glm::vec4(-1.0f, 1.0f, 1.0f + padding, 1.0f),

			current_preference_settings.hidden_cube_colour
		);
	}
}

void CubeletFace::applyTransform(const glm::mat4& transform) {
	for (Vertex& vertex : verticies_)
		vertex.position_ = transform * vertex.position_;
}

/*

void CubeletFace::applyColour(const CubeletFace& face, utils::ArrayList<Vertex>& verticies) const {
	for (unsigned int vertex = 0; vertex < face_indices; vertex++) 
		verticies[renderer_vertex_indicies_[vertex]].colour_ = face.verticies_[vertex].colour_;
}

*/

void CubeletFace::addToRender(utils::ArrayList<Vertex>& verticies, utils::ArrayList<unsigned int>& indices) {
	unsigned int quads_in_verticies = verticies.getSize() / EDGES_ON_QUAD;
	
	for (unsigned int vertex = 0; vertex < face_indices; vertex++)
		renderer_vertex_indicies_.add(verticies.getSize() + vertex);

	verticies.add(verticies_);

	for (unsigned int quad = 0; quad < verticies_.getSize() / EDGES_ON_QUAD; quad++)
		for (unsigned int index = 0; index < EDGES_ON_QUAD + 2; index++)
			indices.add(quad_index_distribution[index] + (quads_in_verticies + quad) * EDGES_ON_QUAD);
}

/////////////////////////////////////////////CUBELET////////////////////////////////////////////////////////////////////

//all transformations that map the F face to all other faces
const glm::mat4 cube_face_transforms[NO_FACES] = {
	glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), x_axis_unit), //U
	glm::rotate(glm::mat4(1.0f), glm::radians(90.0f) , y_axis_unit), //R
	glm::rotate(glm::mat4(1.0f), glm::radians(0.0f)  , x_axis_unit), //F
	glm::rotate(glm::mat4(1.0f), glm::radians(90.0f) , x_axis_unit), //D
	glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), y_axis_unit), //L
	glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), y_axis_unit), //B
};

inline bool addFace(utils::ArrayList<CubeletFace>& faces, utils::ArrayList<unsigned int>& coloured_faces, bool coloured, unsigned int index) {
	glm::vec4 colour = current_preference_settings.hidden_cube_colour;

	if (coloured) {
		colour = current_preference_settings.colour_scheme[index];
		coloured_faces.add(index);	
	}

	faces.add(CubeletFace(cube_face_transforms[index], colour));
	return coloured;
}

Cubelet::Cubelet(const glm::vec3& translate): coloured_flags_(0) {

	float abs_max_translate = (1.0f + current_preference_settings.sticker_padding) * 2;

	coloured_flags_ |= addFace(faces_, coloured_faces_, (translate.y / abs_max_translate) ==  1, 0) << U;
	coloured_flags_ |= addFace(faces_, coloured_faces_, (translate.x / abs_max_translate) ==  1, 1) << R;
	coloured_flags_ |= addFace(faces_, coloured_faces_, (translate.z / abs_max_translate) ==  1, 2) << F;
	coloured_flags_ |= addFace(faces_, coloured_faces_, (translate.y / abs_max_translate) == -1, 3) << D;
	coloured_flags_ |= addFace(faces_, coloured_faces_, (translate.x / abs_max_translate) == -1, 4) << L;
	coloured_flags_ |= addFace(faces_, coloured_faces_, (translate.z / abs_max_translate) == -1, 5) << B;

	applyTransform(glm::translate(glm::mat4(1.0f), translate));
}

/*

void Cubelet::changeColours(unsigned int orientation, const Cubelet& cubelet, utils::ArrayList<Vertex>& verticies) const {
	
	unsigned int cubelet_type = coloured_faces_.getSize();

	for (unsigned int face = 0; face < cubelet_type; face++)
		faces_[coloured_faces_[(face + orientation) % cubelet_type]].applyColour(cubelet.faces_[cubelet.coloured_faces_[face]], verticies);
}

*/

void Cubelet::applyTransform(const glm::mat4& transform) {
	for (CubeletFace& face : faces_)
		face.applyTransform(transform);
}

void Cubelet::addToRender(utils::ArrayList<Vertex>& verticies, utils::ArrayList<unsigned int>& indices) {
	for (CubeletFace& face : faces_)
		face.addToRender(verticies, indices);
}

bool Cubelet::isFaceColoured(const Face& face) const {
	return coloured_flags_ & (1 << face);
}


