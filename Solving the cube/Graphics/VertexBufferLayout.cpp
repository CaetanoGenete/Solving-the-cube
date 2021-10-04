#include "VertexBufferLayout.h"

const utils::ArrayList<LayoutElement>& VertexBufferLayout::getElements() const
{
	return elements;
}

unsigned int VertexBufferLayout::getStride() const
{
	return stride;
}
