#pragma once

#include "CUtils.h"

class IndexBuffer {

private:

	unsigned int iboID;
	unsigned int count;

public:

	IndexBuffer();

	IndexBuffer(const utils::ArrayList<unsigned int>& arrayList, unsigned int drawType);
	IndexBuffer(unsigned int count, const unsigned int* data, unsigned int drawType);
	
	~IndexBuffer();

	void bind() const;
	void unBind() const;

	unsigned int getCount() const;

};