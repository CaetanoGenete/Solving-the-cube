#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(): iboID(0), count(0)
{
}

IndexBuffer::IndexBuffer(const utils::ArrayList<unsigned int>& arrayList, unsigned int drawType) :
	IndexBuffer(arrayList.getSize(), arrayList.getData(), drawType)
{
}

IndexBuffer::IndexBuffer(unsigned int count, const unsigned int * data, unsigned int drawType): count(count) {
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawType);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &iboID);
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
}

void IndexBuffer::unBind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() const
{
	return count;
}
