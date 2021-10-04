#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoID);
}

void VertexArray::addBuffer(const VertexBuffer & buffer, const VertexBufferLayout & layout) {
	bind();
	buffer.bind();

	const auto& elements = layout.getElements();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.getSize(); i++) {
		LayoutElement& element = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (const void*)offset);

		offset += element.count * LayoutElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(vaoID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

