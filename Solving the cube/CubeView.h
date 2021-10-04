#pragma once

#include "CUtils.h"

#include "glm\glm.hpp"

#include "Graphics\Vertex.h"
#include "Graphics\Renderer.h"

#include "CubieLevel.h"

#include "Preferences.h"

#include "Window\Window.h"
#include "Events\Event.h"
#include "Events\MouseEvents.h"
#include "Events\KeyEvents.h"

#include "BoundingBox.h"

#include "Camera.h"

#define NO_STICKERS_PER_FACE 9

void StaticVirtual3DCubeInitialise();

class CubieLevel;

class Virtual3DCube {

private:

	//Rendering necessities:

	utils::ArrayList<Vertex> verticies_;
	utils::ArrayList<unsigned int> indicies_;

	VertexArray* vertex_array_;

	VertexBuffer* vertex_buffer_;
	IndexBuffer* indexBuffer_;

	Shader* shaders_;

	//Camera:

	Camera camera_;

	//Cube properties:

	OBoundingBox cube_bounds_;
	CubieLevel cube_data_;

	//Turning faces:

	//bool turning_ = false;
	//Face currently_turning_;

	utils::ArrayList<unsigned char> turn_queue_;

	void rotateFace(float rotation, unsigned char move);

	void resetCube();

public:

	Virtual3DCube(const Window& window);
	~Virtual3DCube();

	void render(const Renderer& renderer);

	bool onMousePressed(MousePressedEvent& event);
	bool onMouseReleased(MouseReleasedEvent& event);
	bool onMouseMoved(MouseMovedEvent& event);

	void onEvent(Event& event);

	void turn(FaceTurn);

	utils::String scramble();
	void solve();
	void hint();

	Camera& camera();
};