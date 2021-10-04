#include "CubeView.h"

#include "glm\gtc\matrix_transform.hpp"
#include "glm\glm.hpp"

#include "Graphics\VertexBufferLayout.h"

#include "CubieLevel.h"
#include "Preferences.h"

#include "GLFW\glfw3.h"

#include "BoundingBox.h"

#include "Cubelet.h"
#include "Scrambler.h"

#include "Solver.h"

#define EDGES_ON_QUAD 4
#define CUBIES_PER_AXIS 3

#define NO_CUBIES CUBIES_PER_AXIS * CUBIES_PER_AXIS * CUBIES_PER_AXIS - (CUBIES_PER_AXIS - 2) * (CUBIES_PER_AXIS - 2) * (CUBIES_PER_AXIS - 2)

Cubelet default_cubies[NO_CUBIES];

const float max_rotation = 90.0f;

const glm::vec3 face_turns_rotation_axes[NO_FACES] = {
	glm::vec3( 0.0f, -1.0f,  0.0f), //U
	glm::vec3(-1.0f,  0.0f,  0.0f), //R
	glm::vec3( 0.0f,  0.0f, -1.0f), //F
	glm::vec3( 0.0f,  1.0f,  0.0f), //D
	glm::vec3( 1.0f,  0.0f,  0.0f), //L
	glm::vec3( 0.0f,  0.0f,  1.0f)  //B
};

const unsigned char corner_cubie_to_cubelet[NO_CORNERS]{
	0, 2, 8, 6, 17, 19, 25, 23
};

const unsigned char edge_cubie_to_cubelet[NO_EDGES]{
	3, 1, 5, 7, 20, 18, 22, 24, 9, 11, 16, 14,
};

const unsigned char center_cubie_to_cubelet[NO_CENTERS]{
	4, 12, 10, 21, 13, 15
};

void StaticVirtual3DCubeInitialise() {

	const float abs_max_translate = (1.0f + current_preference_settings.sticker_padding) * 2;

	const glm::vec4 x_increment = glm::vec4(abs_max_translate, 0.0f, 0.0f, 0.0f);
	const glm::vec4 y_increment = glm::vec4(0.0f, abs_max_translate, 0.0f, 0.0f);
	const glm::vec4 z_increment = glm::vec4(0.0f, 0.0f, abs_max_translate, 0.0f);

	const glm::vec4 standard_translate = x_increment + y_increment + z_increment;

	unsigned int index = 0;

	glm::vec4 y_translate = y_increment;
	for (unsigned int y = 0; y < CUBIES_PER_AXIS; y++, y_translate -= y_increment) {

		glm::vec4 z_translate = z_increment;
		for (unsigned int z = 0; z < CUBIES_PER_AXIS; z++, z_translate -= z_increment) {
			bool z_contained = y > 0 && z > 0 && y < CUBIES_PER_AXIS - 1 && z < CUBIES_PER_AXIS - 1;

			glm::vec4 x_translate = x_increment;
			for (unsigned int x = 0; x < CUBIES_PER_AXIS; x++, x_translate -= x_increment) {
				if (z_contained && x > 0 && x < CUBIES_PER_AXIS - 1)
					continue;

				default_cubies[index++] = Cubelet(x_translate + y_translate + z_translate);
			}
		}
	}
}

Virtual3DCube::Virtual3DCube(const Window& window) :
	camera_(window)
{
	
	//vector pointing from center to vertex of cube.
	glm::vec3 max_bound((1.0f + current_preference_settings.sticker_padding) * CUBIES_PER_AXIS);
	cube_bounds_ = OBoundingBox(-max_bound, max_bound);

	for (unsigned int i = 0; i < NO_CUBIES; i++)
		default_cubies[i].addToRender(verticies_, indicies_);

	vertex_buffer_ = new VertexBuffer(verticies_, GL_STATIC_DRAW);
	indexBuffer_   = new IndexBuffer(indicies_, GL_STATIC_DRAW);

	vertex_array_ = new VertexArray();

	shaders_ = new Shader("res/vertex.shader", "res/fragment.shader");

	//Vertex layout: 4 dimensional position (type float), 4 dimensional colour (type float) 
	VertexBufferLayout layout;
	layout.push<float>(EDGES_ON_QUAD); 
	layout.push<float>(COLOUR_CHANNELS);

	vertex_array_->addBuffer(*vertex_buffer_, layout);
}

Virtual3DCube::~Virtual3DCube()
{
	delete vertex_array_;
	delete vertex_buffer_;
	delete indexBuffer_;
	delete shaders_;
}

void Virtual3DCube::resetCube()
{
	cube_data_ = CubieLevel();
	verticies_.clear();

	utils::ArrayList<unsigned int> dump;

	for (unsigned int i = 0; i < NO_CUBIES; i++)
		default_cubies[i].addToRender(verticies_, dump);

	vertex_buffer_->updateData();
}

inline void turnCubelets(const unsigned char* perm, const unsigned char* conversion, utils::ArrayList<Vertex>& verticies, const Cubelet* cubelets, const glm::mat4& transform, Face face, unsigned int no_cubelets) {
	const unsigned int no_verticies_per_cubelet = verticies.getSize() / 26;

	for (unsigned char cubie = 0; cubie < no_cubelets; cubie++) {
		unsigned char cubelet_index = conversion[cubie];


		if (cubelets[cubelet_index].isFaceColoured(face)) {
			unsigned int moved_cubelet_index = conversion[perm[cubie]];

			unsigned int renderer_start_index = moved_cubelet_index * no_verticies_per_cubelet;
			
			for (unsigned int vertex = 0; vertex < no_verticies_per_cubelet; vertex++) {
				unsigned int renderer_index = renderer_start_index + vertex;

				verticies[renderer_index].position_ = transform * verticies[renderer_index].position_;
			}
		}

	}

}

void Virtual3DCube::rotateFace(float rotation_angle, unsigned char move)
{
	Face currently_turning = (Face)(move % NO_UNIQUE_MOVES);
	
	if (rotation_angle > 180.0f) rotation_angle = 90.0f;
	if (move / NO_UNIQUE_MOVES == 2) rotation_angle *= -1;
	

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle), face_turns_rotation_axes[currently_turning]);

	turnCubelets((unsigned char*)cube_data_.corner_perm(), corner_cubie_to_cubelet, verticies_, default_cubies, rotation, currently_turning, NO_CORNERS);
	turnCubelets((unsigned char*)cube_data_.edge_perm(), edge_cubie_to_cubelet, verticies_, default_cubies, rotation, currently_turning, NO_EDGES);
	turnCubelets((unsigned char*)pristine_center_perm_config, center_cubie_to_cubelet, verticies_, default_cubies, rotation, currently_turning, NO_CENTERS);

	vertex_buffer_->updateData();
}

void Virtual3DCube::render(const Renderer & renderer) {
	static float face_rotation = 0;
	static float max_rotation = 90.0f;

	if (std::abs(face_rotation) >= max_rotation) {
		unsigned char turn = turn_queue_.remove(0);
		cube_data_.turnFace((FaceTurn) turn);
		face_rotation = 0;
	}

	if (turn_queue_.getSize()) {
		if ((turn_queue_[0] / NO_UNIQUE_MOVES) == 1)
			max_rotation = 180.0f;
		else
			max_rotation = 90.0f;

		rotateFace(current_preference_settings.rotation_speed, turn_queue_[0]);
		face_rotation += current_preference_settings.rotation_speed;
	}


	vertex_array_->bind();

	//sends MVP matrix to graphics card 
	shaders_->setUniformMat4("MVP", camera_.getPV() * cube_bounds_.model_matrix_);

	//draws the cube
	renderer.drawElements(*vertex_buffer_, *indexBuffer_, *shaders_);
}

static bool mouse_held_out = false;
static bool mouse_held_on = false;

bool Virtual3DCube::onMousePressed(MousePressedEvent& event) {
	double x = event.x();
	double y = event.y();
	
	glm::vec4 eye_ray = camera_.screenToEyeRay(x, y);
	glm::vec3 world_ray = camera_.eyeRayToWorldRay(eye_ray);
		
	if (cube_bounds_.rayIntersection(camera_.position(), world_ray))
		mouse_held_on = true;
	else
		mouse_held_out = true;

	return false;
}

bool Virtual3DCube::onMouseReleased(MouseReleasedEvent& event) {
	mouse_held_out = false;
	mouse_held_on = false;

	return false;
}

bool Virtual3DCube::onMouseMoved(MouseMovedEvent& event) {
	static float prev_x, prev_y;

	if (mouse_held_out) {
		glm::vec2 drag_vec = glm::vec2(event.x() - prev_x, event.y() - prev_y);

		glm::mat4 x_rotation = glm::rotate(glm::mat4(1.0f), drag_vec.x * current_preference_settings.camera_sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 y_rotation = glm::rotate(glm::mat4(1.0f), drag_vec.y * current_preference_settings.camera_sensitivity, glm::vec3(1.0f, 0.0f, 0.0f));

		cube_bounds_.model_matrix_ = x_rotation * y_rotation * cube_bounds_.model_matrix_;
	}

	if (mouse_held_on) {
		glm::vec3 x_axis = glm::vec3(cube_bounds_.model_matrix_[0].x, cube_bounds_.model_matrix_[0].y, cube_bounds_.model_matrix_[0].z);
		glm::vec3 y_axis = glm::vec3(cube_bounds_.model_matrix_[1].x, cube_bounds_.model_matrix_[1].y, cube_bounds_.model_matrix_[1].z);

	}

	prev_x = event.x();
	prev_y = event.y();

	return false;
}


void Virtual3DCube::onEvent(Event& event) {
	EventDispatcher dispatcher(event);

	dispatcher.dispatch<MousePressedEvent>(std::bind(&Virtual3DCube::onMousePressed, this, std::placeholders::_1));
	dispatcher.dispatch<MouseReleasedEvent>(std::bind(&Virtual3DCube::onMouseReleased, this, std::placeholders::_1));
	dispatcher.dispatch<MouseMovedEvent>(std::bind(&Virtual3DCube::onMouseMoved, this, std::placeholders::_1));
}

void Virtual3DCube::turn(FaceTurn turn)
{
	if (!turn_queue_.getSize())
		turn_queue_.add(turn);
}

utils::String Virtual3DCube::scramble()
{
	if (!turn_queue_.getSize()) {

		resetCube();

		CubieLevel cube;
		scrambleCube(cube);

		std::cout << cube << std::endl;

		utils::ArrayList<unsigned char> scramble = getScrambleString(cube);

		for (unsigned int move = 0; move < scramble.getSize(); move++) { 
			unsigned char actual_move = scramble[move];
				rotateFace(((actual_move / NO_UNIQUE_MOVES) + 1) * 90.0f, actual_move);

			cube_data_.turnFace((FaceTurn)actual_move);
		}


		return scrambleStringToString(scramble);
	}

	return utils::String();
}

void Virtual3DCube::solve()
{
	if (!turn_queue_.getSize()) {
		utils::ArrayList<unsigned char> moves;
		moves = search(cube_data_, 21);

		for (unsigned int i = 0; i < moves.getSize(); i++)
			turn_queue_.add(moves[i]);
		
	}
}

void Virtual3DCube::hint()
{
	if (!turn_queue_.getSize()) {
		utils::ArrayList<unsigned char> moves;
		moves = search(cube_data_, 21);

		if (moves.getSize()) {
			turn_queue_.add(moves[0]);
			turn_queue_.add(getInverseMove(moves[0]));
		}
	}

}

Camera& Virtual3DCube::camera() {
	return camera_;
}