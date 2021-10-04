#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(): vboID(0) {
}

VertexBuffer::VertexBuffer(unsigned int size, const void * data, unsigned int drawType): size(size), data(data), drawType(drawType) {
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &vboID);
}

void VertexBuffer::updateData()
{
	bind();

	glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

void VertexBuffer::unBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

