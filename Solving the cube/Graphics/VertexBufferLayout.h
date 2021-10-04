#pragma once
#include "CUtils.h"

#include "Renderer.h"

struct LayoutElement {
	unsigned int count;
	unsigned int type;
	unsigned int normalised;

	static unsigned int getSizeOfType(unsigned int type) {
		switch (type) {
		case GL_UNSIGNED_INT:
			return 4;
		case GL_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		case GL_BYTE:
			return 1;
		case GL_FLOAT:
			return 4;
		default:
			return 0;
		}
	}
};

class VertexBufferLayout {

private:

	utils::ArrayList<LayoutElement> elements;

	unsigned int stride;

public:

	VertexBufferLayout(): stride(0) {

	}

	template<typename T>
	void push(unsigned int count) {
		std::cout << "ERROR:UNKNOWN TYPE" << std::endl;
	}

	template<>
	void push<unsigned int>(unsigned int count) {
		elements.add({ count, GL_UNSIGNED_INT, GL_FALSE });

		stride += count * LayoutElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<int>(unsigned int count) {
		elements.add({ count, GL_INT, GL_FALSE });

		stride += count * LayoutElement::getSizeOfType(GL_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count) {
		elements.add({ count, GL_UNSIGNED_BYTE, GL_FALSE });

		stride += count * LayoutElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	template<>
	void push<char>(unsigned int count) {
		elements.add({ count, GL_BYTE, GL_FALSE });

		stride += count * LayoutElement::getSizeOfType(GL_BYTE);
	}

	template<>
	void push<float>(unsigned int count) {
		elements.add({ count, GL_FLOAT, GL_FALSE });

		stride += count * LayoutElement::getSizeOfType(GL_FLOAT);
	}

	const utils::ArrayList<LayoutElement>& getElements() const;

	unsigned int getStride() const;
};