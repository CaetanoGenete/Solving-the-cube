#pragma once

#include "CUtils.h"

class VertexBuffer {

private:

	unsigned int vboID;

	unsigned int size;
	const void* data;
	unsigned int drawType;

public:

	VertexBuffer();
	
	template<class T>
	VertexBuffer(const utils::ArrayList<T>& arrayList, unsigned int drawType) :
		VertexBuffer(sizeof(T) * arrayList.getSize(), arrayList.getData(), drawType)
	{

	}

	VertexBuffer(unsigned int size, const void* data, unsigned int drawType);
	
	~VertexBuffer();

	void updateData();

	void bind() const;
	void unBind() const;

};