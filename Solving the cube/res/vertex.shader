#version 330 core

in layout(location = 0) vec4 position_in;
in layout(location = 1) vec4 colour_in;

uniform mat4 MVP;

out vec4 colour;

void main() {
	gl_Position = MVP * position_in;

	colour = colour_in;
}