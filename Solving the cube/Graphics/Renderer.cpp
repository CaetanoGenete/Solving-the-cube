#include "Renderer.h"

static bool glew_initialised = false;

Renderer::Renderer() {
	if (!glew_initialised)
		glew_initialised = glewInit();
}

void Renderer::drawElements(const VertexBuffer & vBuffer, const IndexBuffer & iBuffer, const Shader& shader) const {
	vBuffer.bind();
	iBuffer.bind();
	shader.bind();

	glDrawElements(GL_TRIANGLES, iBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
}

