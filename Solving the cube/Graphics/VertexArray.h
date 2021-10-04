#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray {
private:
	unsigned int vaoID;

public:

	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};