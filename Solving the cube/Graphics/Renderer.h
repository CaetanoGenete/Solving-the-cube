#pragma once

#include "GL\glew.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

class Renderer {
private:
	
	unsigned int renderID;

public:

	Renderer();

	void drawElements(const VertexBuffer& vBuffer, const IndexBuffer& iBuffer, const Shader& shader) const;

};